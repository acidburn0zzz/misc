BEGIN TRANSACTION;

DROP TABLE IF EXISTS db_sequence;
DROP TABLE IF EXISTS exposant;

CREATE TABLE db_sequence (
    name TEXT PRIMARY KEY,
    seq NUMBER DEFAULT 100
);

CREATE TABLE exposant (
    id NUMBER PRIMARY KEY,
    nom TEXT,
    domaine NUMBER,
    resp_nom TEXT,
    resp_prenom TEXT,
    email TEXT,
    inscription TEXT,
    passwd TEXT,
    CONSTRAINT c_name UNIQUE (nom, resp_nom, resp_prenom)
);

INSERT INTO db_sequence (name) VALUES ('exposant');

--Valeurs test
INSERT INTO exposant (id, nom, inscription, passwd) VALUES (100, 'Sun Microsystems', datetime('now'), 'ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb');

COMMIT;
