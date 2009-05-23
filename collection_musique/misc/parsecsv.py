#! /usr/bin/python
# -*- encoding: utf-8 -*-

#Les albums
inf = open('albums.csv')
l = inf.readlines()
inf.close()

out_sql = open('db.sql', 'w')
out_sql.write("BEGIN TRANSACTION;\n\n")
out_sql.write("DROP TABLE IF EXISTS albums;\n")
out_sql.write("CREATE TABLE albums (id INTEGER PRIMARY KEY, artist TEXT, album TEXT, year INTEGER, genre TEXT, tracks INTEGER, length TEXT);\n")
out_sql.write("CREATE UNIQUE INDEX album_id ON albums (artist, album);\n\n")

for i in l:
    s = i[:-1].split(';'); #on enleve le \n en meme temps
    if len(s) != 6:
        continue
    s[0] = s[0].replace("'", "''")
    s[1] = s[1].replace("'", "''")
    out_sql.write("INSERT INTO albums (artist, album, year, genre, tracks, length) VALUES ('" + s[0] + "', '" + s[1] + "', " + s[2] + ", '" + s[3] + "', " + s[4] + ", '" + s[5] + "');\n")

out_sql.write("\nCOMMIT;\n")
out_sql.close()
