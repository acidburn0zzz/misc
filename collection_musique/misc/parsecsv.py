#! /usr/bin/python
# -*- encoding: utf-8 -*-

import os

inf = open('liste.csv')
l = inf.readlines()
inf.close()

#~ out_sh = open('liste.sh', 'w')
#~ out_sh.write("#! /bin/bash\n\n")
#~ out_sh.write("sqlite3 music.db << EOF\n")
#~ out_sh.write("    CREATE TABLE collection (id INTEGER PRIMARY KEY, artiste TEXT, album TEXT, annee INTEGER, genre TEXT, pistes INTEGER, duree INTEGER);\n")

out_sql = open('liste.sql', 'w')
out_sql.write("BEGIN TRANSACTION;\n")
out_sql.write("CREATE TABLE collection (id INTEGER PRIMARY KEY, artiste TEXT, album TEXT, annee INTEGER, genre TEXT, pistes INTEGER, duree INTEGER);\n")
out_sql.write("CREATE UNIQUE INDEX album_id ON collection (artiste, album);\n")

for i in l:
    s = i[:-1].split('|'); #on enleve le \n en meme temps
    s[1] = s[1].replace("'", "''")
    s[5] = str(int(s[5][0:2]) * 3600 + int(s[5][3:5]) * 60 + int(s[5][6:8]))
    #~ out_sh.write("    INSERT INTO collection (artiste, album, annee, genre, pistes, duree) VALUES ('" + s[0] + "', '" + s[1] + "', " + s[2] + ", '" + s[3] + "', " + s[4] + ", " + s[5] + ");\n")
    out_sql.write("INSERT INTO collection (artiste, album, annee, genre, pistes, duree) VALUES ('" + s[0] + "', '" + s[1] + "', " + s[2] + ", '" + s[3] + "', " + s[4] + ", " + s[5] + ");\n")

#~ out_sh.write("EOF\n")
out_sql.write("COMMIT;\n")

#~ out_sh.close()
out_sql.close()

#~ os.system("chmod +x liste.sh")
os.system("rm ../music.db; sqlite3 ../music.db < liste.sql")
