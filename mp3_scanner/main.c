#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <sys/stat.h>
#include <crc32.h>

#include <sqlite3.h>

#include "id3.h"

/*
int main(int argc, char **argv) {
    id3_file_s *id3f;
    id3v1_s *id3;

    id3f = id3_file_open(argv[1]);
    id3 = id3_file_read(id3f);

    if (id3f->isID3)
        printf("Artist: %.30s\n", id3->artist);

    id3_file_close(id3f);

    return EXIT_SUCCESS;
}
*/

sqlite3 *db;

int list(const char *name, const struct stat *status, int type);
void init_db();

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

    init_db();

    ftw(argv[1], list, 1);

    sqlite3_close(db);
    return 0;
}

void init_db() {
    sqlite3_exec(db, "DROP TABLE IF EXISTS songs;", NULL, 0, NULL);
    sqlite3_exec(db, "VACUUM;", NULL, 0, NULL);
    sqlite3_exec(db, "CREATE TABLE songs (id INTEGER PRIMARY KEY, file TEXT, artist TEXT, album TEXT, year INTEGER, genre INTEGER, track INTEGER, crc32 TEXT);", NULL, 0, NULL);
}

int list(const char *name, const struct stat *status, int type) {
    id3_file_s *id3f;
    id3v1_s *id3;
    char sql[4096];

    if (type != FTW_F)
        return 0;

    if (strncmp(name + strlen(name) - 4, ".mp3", 4) != 0)
        return 0;

    id3f = id3_file_open((char*)name);
    id3 = id3_file_read(id3f);
    id3_file_close(id3f);

    if (!id3f->isID3)
        return 0;

    sprintf(sql, "INSERT INTO songs (file, artist, album, year, genre, track, crc32) VALUES (\"%s\", \"%.30s\", \"%.30s\", %.4s, %d, %d, \"%s\");",
        name, id3->artist, id3->album, id3->year, id3->genre, id3->track, "0");
    /*puts(sql);*/
    sqlite3_exec(db, sql, NULL, 0, NULL);

    return 0;
}
