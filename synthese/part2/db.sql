BEGIN TRANSACTION;

DROP TABLE IF EXISTS db_sequence;
DROP TABLE IF EXISTS domaines;
DROP TABLE IF EXISTS exposants;
DROP TABLE IF EXISTS zones;

CREATE TABLE db_sequence (
    name TEXT PRIMARY KEY,
    seq INTEGER DEFAULT 100
);

CREATE TABLE domaines (
    id INTEGER PRIMARY KEY,
    nom TEXT,
    CONSTRAINT c_name UNIQUE (nom)
);

CREATE TABLE exposants (
    id          INTEGER PRIMARY KEY,
    nom         TEXT,
    domaine     INTEGER,
    resp_nom    TEXT,
    resp_prenom TEXT,
    email       TEXT,
    date_inscr  TEXT,
    passwd      TEXT,
    CONSTRAINT c_name UNIQUE (nom, resp_nom, resp_prenom)
);

CREATE TABLE zones (
    id          INTEGER PRIMARY KEY AUTOINCREMENT,
    posx        INTEGER,
    posy        INTEGER,
    proprio     INTEGER,
    webz        INTEGER,
    router      INTEGER,
    electricite INTEGER,
    murets      INTEGER,
    murs        INTEGER--,
    --Pas de contrainte ici pour avoir un historique de chaque zone
    --CONSTRAINT c_name UNIQUE (posx, posy)
);

CREATE TRIGGER update_id_exposant AFTER INSERT ON exposants BEGIN
    UPDATE db_sequence SET seq=seq+10 WHERE name='exposants';
END;

INSERT INTO db_sequence (name) VALUES ('exposants');

--Valeurs par defaut
INSERT INTO domaines (nom) VALUES ('Mécanique');
INSERT INTO domaines (nom) VALUES ('Physique');
INSERT INTO domaines (nom) VALUES ('Medical');
INSERT INTO domaines (nom) VALUES ('Pharmaceutique');
INSERT INTO domaines (nom) VALUES ('Électronique');
INSERT INTO domaines (nom) VALUES ('Informatique');
INSERT INTO domaines (nom) VALUES ('Enseignement');
INSERT INTO domaines (nom) VALUES ('Autre');
--Valeurs par defaut

--Exposant special : Admin
INSERT INTO exposants (id, nom, domaine, resp_nom, resp_prenom, email, date_inscr, passwd) VALUES
    (1, 'Administration', 8, 'Lemay', 'Mathieu', 'acidrain1@gmail.com', datetime('now'),
    '59a4b6f06161d9491685779bff25db62ec66efa708b922e57bed29086d2d1026dcd3abc63e0f5c25aedb752156795b881b434d2020b7cfb36529864cfe9d4eaa');

--Valeurs test
INSERT INTO exposants (id, nom, domaine, resp_nom, resp_prenom, email, date_inscr, passwd) VALUES
    (100, 'Mozilla', 6, 'Lemay', 'Mathieu', 'acidrain1@gmail.com', datetime('now'),
    '1f40fc92da241694750979ee6cf582f2d5d7d28e18335de05abc54d0560e0f5302860c652bf08d560252aa5e74210546f369fbbbce8c12cfc7957b2652fe9a75');
INSERT INTO exposants (id, nom, domaine, resp_nom, resp_prenom, email, date_inscr, passwd) VALUES
    (110, 'IBM', 5, 'Li', 'Olivier', 'te_addict@gmail.com', datetime('now'),
    '5267768822ee624d48fce15ec5ca79cbd602cb7f4c2157a516556991f22ef8c7b5ef7b18d1ff41c59370efb0858651d44a936c11b7b144c48fe04df3c6a3e8da');
INSERT INTO exposants (id, nom, domaine, resp_nom, resp_prenom, email, date_inscr, passwd) VALUES
    (120, 'Russia', 7, 'Parisien', 'Maxime', 'mpalladini@gmail.com', datetime('now'),
    'acc28db2beb7b42baa1cb0243d401ccb4e3fce44d7b02879a52799aadff541522d8822598b2fa664f9d5156c00c924805d75c3868bd56c2acb81d37e98e35adc');

INSERT INTO zones (posx, posy, proprio, webz, router, electricite, murets) VALUES
    (1, 3, 100, 3, 1, 1, 214);
    
INSERT INTO zones (posx, posy, proprio, webz, router, electricite, murets) VALUES
    (1, 1, 110, 0, 0, 1, 147);
    
INSERT INTO zones (posx, posy, proprio, webz, router, electricite, murets) VALUES
    (5, 3, 120, 1, 1, 2, 48);
--Valeurs test

COMMIT;
