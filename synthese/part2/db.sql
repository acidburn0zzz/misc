
BEGIN TRANSACTION;

DROP TABLE IF EXISTS db_sequence;
DROP TABLE IF EXISTS exposants;

CREATE TABLE db_sequence (
    name TEXT PRIMARY KEY,
    seq NUMBER DEFAULT 100
);

CREATE TABLE exposants (
    id NUMBER PRIMARY KEY,
    nom TEXT,
    domaine NUMBER,
    resp_nom TEXT,
    resp_prenom TEXT,
    email TEXT,
    date_inscr TEXT,
    passwd TEXT,
    CONSTRAINT c_name UNIQUE (nom, resp_nom, resp_prenom)
);

INSERT INTO db_sequence (name) VALUES ('exposants');

--Valeurs test
INSERT INTO exposants (id, nom, date_inscr, passwd) VALUES (100, 'Sun Microsystems', datetime('now'), '1f40fc92da241694750979ee6cf582f2d5d7d28e18335de05abc54d0560e0f5302860c652bf08d560252aa5e74210546f369fbbbce8c12cfc7957b2652fe9a75');

COMMIT;
