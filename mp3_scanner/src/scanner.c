#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <pcre.h>
#include <sys/stat.h>
#include <sqlite3.h>

#include <FLAC/metadata.h>
#include <id3tag.h>

#include "genres.h"

struct tag_s {
    char *fn;
    char *artist;
    char *album;
    char *title;
    char *year;
    char *genre;
    char *track;
    char *type;
    long lastupdate;
};

typedef struct tag_s tag_t;

sqlite3 *db;
pcre *reg_mp3;
pcre *reg_flac;
pcre *reg_num;
const char *error;
int erroffset;

int init();
int list(const char *name, const struct stat *status, int type);
void insert_song(tag_t tag);

void read_mp3(const char *fn);
void read_flac(const char *fn);

char *get_id3_tag(struct id3_tag *tag, const char *id);

char *trim(char *s);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "%s\nBy: %s\n", PACKAGE_STRING, PACKAGE_BUGREPORT);
        fprintf(stderr, "Usage: %s <dir> <db_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (sqlite3_open(argv[2], &db) != 0) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        exit(EXIT_FAILURE);
    }

    if (init() != 0) {
        exit(EXIT_FAILURE);
    }

    sqlite3_exec(db, "BEGIN;", NULL, 0, NULL);
    ftw(argv[1], list, 1);

    sqlite3_exec(db, "COMMIT;", NULL, 0, NULL);
    sqlite3_close(db);
    return EXIT_SUCCESS;
}

int init() {
    int ret;

    ret = sqlite3_exec(db, "DROP TABLE IF EXISTS songs", NULL, 0, NULL);
    if (ret != SQLITE_OK) {
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
        exit(-1);
    }

    ret = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS songs (\
            id INTEGER PRIMARY KEY, file TEXT, artist TEXT, album TEXT, title TEXT, year INTEGER, genre TEXT, track TEXT, type TEXT, lastupdate NUMBER, \
            UNIQUE (artist, album, title));", NULL, 0, NULL);
    if (ret != SQLITE_OK) {
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
        exit(-1);
    }

    reg_mp3 = pcre_compile("\\.mp3$", 0, &error, &erroffset, NULL);
    if (reg_mp3 == NULL) {
        fprintf(stderr, "%s at %d\n", error, erroffset);
        return -1;
    }

    reg_flac = pcre_compile("\\.flac$", 0, &error, &erroffset, NULL);
    if (reg_flac == NULL) {
        fprintf(stderr, "%s at %d\n", error, erroffset);
        return -1;
    }

    reg_num = pcre_compile("^[0-9]+$", 0, &error, &erroffset, NULL);
    if (reg_num == NULL) {
        fprintf(stderr, "%s at %d\n", error, erroffset);
        return -1;
    }

    return 0;
}

int list(const char *fn, const struct stat *status, int type) {
    if (type == FTW_D) {
        printf("Processing %s\n", fn);
    }
    if (type != FTW_F) {
        return 0;
    }

    if (pcre_exec(reg_mp3, NULL, fn, strlen(fn), 0, 0, NULL, 0) == 0) {
        read_mp3(fn);
    } else if (pcre_exec(reg_flac, NULL, fn, strlen(fn), 0, 0, NULL, 0) == 0) {
        read_flac(fn);
    }

    return 0;
}

void read_mp3(const char *fn) {
    struct id3_file *file;
    struct id3_tag *id3;
    tag_t tag;
    int genre;

    file = id3_file_open(fn, ID3_FILE_MODE_READONLY);
    id3 = id3_file_tag(file);

    tag.fn = fn;
    tag.artist = get_id3_tag(id3, ID3_FRAME_ARTIST);
    tag.album = get_id3_tag(id3, ID3_FRAME_ALBUM);
    tag.title = get_id3_tag(id3, ID3_FRAME_TITLE);
    tag.year = get_id3_tag(id3, ID3_FRAME_YEAR);
    tag.genre = get_id3_tag(id3, ID3_FRAME_GENRE);
    tag.track = get_id3_tag(id3, ID3_FRAME_TRACK);
    tag.type = "mp3";
    tag.lastupdate = 42;

    if (pcre_exec(reg_num, NULL, tag.genre, strlen(tag.genre), 0, 0, NULL, 0) == 0) {
        genre = atoi(tag.genre);
        tag.genre = id3_genres[genre];
    }

    insert_song(tag);

    id3_file_close(file);
}

void read_flac(const char *fn) {
    FLAC__StreamMetadata *tags;
    FLAC__StreamMetadata_VorbisComment vorbis_comment;
    FLAC__StreamMetadata_VorbisComment_Entry entry;
    tag_t tag;
    int i;

    char delim[] = "=";
    char *tagname, *value;

    if (FLAC__metadata_get_tags(fn, &tags) == false || tags->type != FLAC__METADATA_TYPE_VORBIS_COMMENT) {
        fprintf(stderr, "%s: No VORBIS_COMMENT found\n", fn);
        FLAC__metadata_object_delete(tags);
        return;
    }

    vorbis_comment = tags->data.vorbis_comment;

    for (i=0; i<vorbis_comment.num_comments; i++) {
        entry = vorbis_comment.comments[i];
        tagname = strtok((char*)entry.entry, delim);
        value = strtok(NULL, delim);

        if (strcmp(tagname, "ALBUM") == 0) {
            tag.album = value;
        } else if (strcmp(tagname, "ARTIST") == 0) {
            tag.artist = value;
        } else if (strcmp(tagname, "TITLE") == 0) {
            tag.title = value;
        } else if (strcmp(tagname, "DATE") == 0) {
            tag.year = value;
        } else if (strcmp(tagname, "GENRE") == 0) {
            tag.genre = value;
        } else if (strcmp(tagname, "TRACKNUMBER") == 0) {
            tag.track = value;
        }
    }

    tag.fn = fn;
    tag.type = "flac";

    insert_song(tag);

    FLAC__metadata_object_delete(tags);
}

void insert_song(tag_t tag) {
    char *sql;

    sql = sqlite3_mprintf("INSERT OR IGNORE INTO songs (file, artist, album, title, year, genre, track, type, lastupdate) VALUES (%Q, %Q, %Q, %Q, %.4Q, %Q, %Q, %Q, %d);",
                          tag.fn, trim(tag.artist), trim(tag.album), trim(tag.title), trim(tag.year), trim(tag.genre), trim(tag.track), tag.type, 0 /*tag.lastupdate*/);

    if (sqlite3_exec(db, sql, NULL, 0, NULL) != SQLITE_OK) {
        fprintf(stderr, "%s\n%s\n", sql, sqlite3_errmsg(db));
    }

    sqlite3_free(sql);
}

char *get_id3_tag(struct id3_tag *tag, const char *id) {
    struct id3_frame *frame;
    union id3_field *field;
    id3_ucs4_t *value;

    if ((frame = id3_tag_findframe(tag, id, 0)) == NULL)
        return NULL;

    if ((field = id3_frame_field(frame, 1)) == NULL)
        return NULL;

    //TODO: Cat if multiple values
    value = id3_field_getstrings(field, 0);

    return id3_ucs4_utf8duplicate(value);
}

char *trim(char *s) {
    int i = 0,j;

    if (s == NULL)
        return NULL;

    /* Trim spaces and tabs from beginning: */
    while ((s[i]==' ') || (s[i]=='\t')) {
        i++;
    }

    if (i>0) {
        for (j=0; j<strlen(s); j++) {
            s[j] = s[j+i];
        }
        s[j] = '\0';
    }

    /* Trim spaces and tabs from end: */
    i = strlen(s)-1;
    while ((s[i]==' ') || (s[i]=='\t')) {
        i--;
    }

    if (i < (strlen(s)-1)) {
        s[i+1] = '\0';
    }

    return s;
}
