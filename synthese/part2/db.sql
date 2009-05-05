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
    password TEXT,
    CONSTRAINT c_name UNIQUE (nom, resp_nom, resp_prenom)
);

INSERT INTO db_sequence (name) VALUES ('exposant');

COMMIT;
