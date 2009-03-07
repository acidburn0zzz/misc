#! /usr/bin/python
# -*- encoding: utf-8 -*-

import os

inf = open('liste.csv')
l = inf.readlines()
inf.close()

out = open('liste', 'w')
out.write("#! /bin/bash\n\n")
out.write("sqlite3 music.db \"DROP TABLE collection;\" > /dev/null\n")
out.write("sqlite3 music.db \"CREATE TABLE collection (id INTEGER PRIMARY KEY, artiste TEXT, album TEXT, annee INT, genre TEXT, pistes INT, duree INT);\" > /dev/null\n\n")

for i in l:
    s = i[:-1].split('|'); #on enleve le \n en meme temps
    s[1] = s[1].replace("'", "''")
    s[5] = str(int(s[5][0:2]) * 3600 + int(s[5][3:5]) * 60 + int(s[5][6:8]))
    out.write("sqlite3 music.db \"INSERT INTO collection (artiste, album, annee, genre, pistes, duree) VALUES ('" + s[0] + "', '" + s[1] + "', " + s[2] + ", '" + s[3] + "', " + s[4] + ", " + s[5] + ");\" > /dev/null || exit -1\n")

os.system("chmod +x liste")
