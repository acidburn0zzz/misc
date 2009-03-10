#! /usr/bin/python
# -*- encoding: utf-8 -*-

import os

#Les albums
inf = open('albums.csv')
l = inf.readlines()
inf.close()

out_sql = open('db.sql', 'w')
out_sql.write("BEGIN TRANSACTION;\n")
out_sql.write("CREATE TABLE collection (id INTEGER PRIMARY KEY, artiste TEXT, album TEXT, annee INTEGER, genre TEXT, pistes INTEGER, duree INTEGER);\n")
out_sql.write("CREATE UNIQUE INDEX album_id ON collection (artiste, album);\n")

for i in l:
    s = i[:-1].split('|'); #on enleve le \n en meme temps
    if len(s) != 6:
        continue
    s[1] = s[1].replace("'", "''")
    s[5] = str(int(s[5][0:2]) * 3600 + int(s[5][3:5]) * 60 + int(s[5][6:8]))
    out_sql.write("INSERT INTO collection (artiste, album, annee, genre, pistes, duree) VALUES ('" + s[0] + "', '" + s[1] + "', " + s[2] + ", '" + s[3] + "', " + s[4] + ", " + s[5] + ");\n")

#Les users
inf = open('users.csv')
l = inf.readlines()
inf.close()

out_sql.write("CREATE TABLE users (id INTEGER PRIMARY KEY, username TEXT, password TEXT, account_type TEXT);\n")
out_sql.write("CREATE UNIQUE INDEX user_id ON users (username);\n")

for i in l:
    s = i[:-1].split('|'); #on enleve le \n en meme temps
    if len(s) != 3:
        continue
    out_sql.write("INSERT INTO users (username, password, account_type) VALUES ('" + s[0] + "', '" + s[1] + "', '" + s[2] + "');\n")

out_sql.write("COMMIT;\n")

out_sql.close()

os.system("rm ../music.db; sqlite3 ../music.db < db.sql")
