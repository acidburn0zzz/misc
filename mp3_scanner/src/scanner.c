#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ftw.h>
#include <pcre.h>
#include <sys/stat.h>
#include <sqlite3.h>

#include <taglib/tag_c.h>

#include <config.h>

#include "genres.h"

struct tag_s {
    char *fn;
    char *artist;
    char *album;
    char *title;
    unsigned int year;
    char *genre;
    unsigned int track;
    char *type;
    time_t lastupdate;
};

typedef struct tag_s tag_t;

sqlite3 *db;
pcre *reg_num;
const char *error;
int erroffset;

int init();
int list(const char *name, const struct stat *status, int type);
void insert_song(tag_t tag);

void read_file(const char *fn, const struct stat *status);
int was_modified(const char *fn, const struct stat *status);

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

/*
    ret = sqlite3_exec(db, "DROP TABLE IF EXISTS songs", NULL, 0, NULL);
    if (ret != SQLITE_OK) {
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
        exit(-1);
    }
*/

    ret = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS songs (\
            id INTEGER PRIMARY KEY, file TEXT, artist TEXT, album TEXT, title TEXT, year INTEGER, genre TEXT, track TEXT, type TEXT, lastupdate NUMBER, \
            UNIQUE (artist, album, title));", NULL, 0, NULL);
    if (ret != SQLITE_OK) {
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
        exit(-1);
    }

    ret = sqlite3_exec(db, "CREATE VIEW IF NOT EXISTS albums AS\
            SELECT artist, album, count(*) songs, year, genre FROM songs GROUP BY album", NULL, 0, NULL);
    if (ret != SQLITE_OK) {
        fprintf(stderr, "%s\n", sqlite3_errmsg(db));
        exit(-1);
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

    if (!was_modified(fn, status))
        return 0;

    read_file(fn, status);

    return 0;
}

void read_file(const char *fn, const struct stat *status) {
    TagLib_File *file;
    TagLib_Tag *tlib_tag;
    tag_t tag;

    file = taglib_file_new(fn);
    if (file == NULL) {
        perror(fn);
        return;
    }

    tlib_tag = taglib_file_tag(file);

    if (tlib_tag != NULL) {
        tag.fn = (char *) fn;
        tag.artist = taglib_tag_artist(tlib_tag);
        tag.album = taglib_tag_album(tlib_tag);
        tag.title = taglib_tag_title(tlib_tag);
        tag.year = taglib_tag_year(tlib_tag);
        tag.genre = taglib_tag_genre(tlib_tag);
        tag.track = taglib_tag_track(tlib_tag);
        tag.lastupdate = status->st_mtime;

        if (strcmp(fn + strlen(fn) - 4, ".mp3") == 0)
            tag.type = "mp3";
        else if (strcmp(fn + strlen(fn) - 4, ".ogg") == 0)
            tag.type = "ogg";
        else if (strcmp(fn + strlen(fn) - 5, ".flac") == 0)
            tag.type = "flac";

        insert_song(tag);
    } else {
        printf("No tag in file: %s\n", fn);
    }

    taglib_tag_free_strings();
    taglib_file_free(file);
}

int was_modified(const char *fn, const struct stat *status) {
    /*
    struct stat st;
    stat(fn, &st);

    fprintf(stderr, "%s: %d\n", fn, st.st_mtime);

    return st.st_mtime;
    */
    return 1;
}

void insert_song(tag_t tag) {
    char *sql;

    sql = sqlite3_mprintf("INSERT OR IGNORE INTO songs (file, artist, album, title, year, genre, track, type, lastupdate) VALUES (%Q, %Q, %Q, %Q, %.4d, %Q, %d, %Q, %d);",
                          tag.fn, trim(tag.artist), trim(tag.album), trim(tag.title), tag.year, trim(tag.genre), tag.track, tag.type, tag.lastupdate);

    if (sqlite3_exec(db, sql, NULL, 0, NULL) != SQLITE_OK) {
        fprintf(stderr, "%s\n%s\n", sql, sqlite3_errmsg(db));
    }

    sqlite3_free(sql);
}

char *trim(char *str) {
    size_t len = 0;
    char *frontp = str - 1;
    char *endp = NULL;

    if (str == NULL)
        return NULL;

    if (str[0] == '\0')
        return str;

    len = strlen(str);
    endp = str + len;

    /* Move the front and back pointers to address
     * the first non-whitespace characters from
     * each end.
     */
    while (isspace(*(++frontp)));
    while (isspace(*(--endp)) && endp != frontp);

    if (str + len - 1 != endp)
        *(endp + 1) = '\0';
    else if (frontp != str &&  endp == frontp)
        *str = '\0';

    /* Shift the string so that it starts at str so
     * that if it's dynamically allocated, we can
     * still free it on the returned pointer.  Note
     * the reuse of endp to mean the front of the
     * string buffer now.
     */
    endp = str;
    if (frontp != str) {
        while(*frontp) *endp++ = *frontp++;
        *endp = '\0';
    }

    return str;
}
