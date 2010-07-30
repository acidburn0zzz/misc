#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <pcre.h>
#include <sys/stat.h>
#include <sqlite3.h>

#include <FLAC/metadata.h>
#include <id3.h>

sqlite3 *db;
pcre *reg_mp3;
pcre *reg_flac;
const char *error;
int erroffset;

int init();
int list(const char *name, const struct stat *status, int type);
void insert_song(const char *fn, char *artist, char *album, char *title, char *year, char *genre, char *track);

void read_mp3(const char *fn);
void read_flac(const char *fn);

char *trim(char *s);

int main(int argc, char *argv[]) {
    if (argc != 3) {
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
            id INTEGER PRIMARY KEY, file TEXT, artist TEXT, album TEXT, title TEXT, year INTEGER, genre TEXT, track INTEGER, \
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

    return 0;
}

int list(const char *fn, const struct stat *status, int type) {
    if (type == FTW_D) {
        printf("Processing %s\n", fn);
    } if (type != FTW_F) {
        return 0;
    }

    if (pcre_exec(reg_mp3, NULL, fn, strlen(fn), 0, 0, NULL, 0) == 0) {
        read_mp3(fn);
    } else if (pcre_exec(reg_flac, NULL, fn, strlen(fn), 0, 0, NULL, 0) == 0) {
        read_flac(fn);
    } else {
        return 0;
    }

    return 0;
}

void read_mp3(const char *fn) {
    ID3 *id3 = NULL;
    int ret;

    id3 = create_ID3(NULL);

    ret = parse_file_ID3(id3, (char*)fn);

    switch(ret) {
        case ID3_OK:
            insert_song(fn, id3->artist, id3->album, id3->title, id3->year, id3->genre, id3->track);
            break;
        case ID3_ERR_EMPTY_FILE:
            fprintf(stderr, "%s: File is empty\n", fn);
            break;
        case ID3_ERR_NO_TAG:
            fprintf(stderr, "%s: No tags found\n", fn);
            break;
        case ID3_ERR_UNSUPPORTED_FORMAT:
            fprintf(stderr, "%s: Unsupported format\n", fn);
            break;
    }

    destroy_ID3(id3);
}

void read_flac(const char *fn) {
    FLAC__StreamMetadata *tags;
    FLAC__StreamMetadata_VorbisComment vorbis_comment;
    FLAC__StreamMetadata_VorbisComment_Entry entry;
    int i;

    char delim[] = "=";
    char *tag, *value;

    char *artist = NULL, *album = NULL, *title = NULL, *year = NULL, *genre = NULL, *track = NULL;

    if (FLAC__metadata_get_tags(fn, &tags) == false || tags->type != FLAC__METADATA_TYPE_VORBIS_COMMENT) {
        fprintf(stderr, "%s: No VORBIS_COMMENT found\n", fn);
        FLAC__metadata_object_delete(tags);
        return;
    }

    vorbis_comment = tags->data.vorbis_comment;

    for (i=0; i<vorbis_comment.num_comments; i++) {
        entry = vorbis_comment.comments[i];
        tag = strtok((char*)entry.entry, delim);
        value = strtok(NULL, delim);

        if (strcmp(tag, "ALBUM") == 0) {
            album = value;
        } else if (strcmp(tag, "ARTIST") == 0) {
            artist = value;
        } else if (strcmp(tag, "TITLE") == 0) {
            title = value;
        } else if (strcmp(tag, "DATE") == 0) {
            year = value;
        } else if (strcmp(tag, "GENRE") == 0) {
            genre = value;
        } else if (strcmp(tag, "TRACKNUMBER") == 0) {
            track = value;
        }
    }

    insert_song(fn, artist, album, title, year, genre, track);

    FLAC__metadata_object_delete(tags);
}

void insert_song(const char *fn, char *artist, char *album, char *title, char *year, char *genre, char *track) {
    char *sql;

    sql = sqlite3_mprintf("INSERT OR IGNORE INTO songs (file, artist, album, title, year, genre, track) VALUES (%Q, %Q, %Q, %Q, %.4Q, %Q, %Q);",
        fn, trim(artist), trim(album), trim(title), trim(year), trim(genre), track);

    if (sqlite3_exec(db, sql, NULL, 0, NULL) != SQLITE_OK) {
        fprintf(stderr, "%s\n%s\n", sql, sqlite3_errmsg(db));
    }

    sqlite3_free(sql);
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
