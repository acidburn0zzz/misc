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

CREATE TRIGGER update_id_exposant AFTER INSERT ON exposants BEGIN
    UPDATE db_sequence SET seq=seq+10 WHERE name='exposants';
END;

INSERT INTO db_sequence (name) VALUES ('exposants');

--Valeurs test
INSERT INTO exposants (id, nom, domaine, resp_nom, resp_prenom, email, date_inscr, passwd) VALUES
    (100, 'Mozilla', 'Dom1', 'Lemay', 'Mathieu', 'acidrain1@gmail.com', datetime('now'),
    '1f40fc92da241694750979ee6cf582f2d5d7d28e18335de05abc54d0560e0f5302860c652bf08d560252aa5e74210546f369fbbbce8c12cfc7957b2652fe9a75');
INSERT INTO exposants (id, nom, domaine, resp_nom, resp_prenom, email, date_inscr, passwd) VALUES
    (110, 'IBM', 'Dom2', 'Li', 'Olivier', 'te_addict@gmail.com', datetime('now'),
    '5267768822ee624d48fce15ec5ca79cbd602cb7f4c2157a516556991f22ef8c7b5ef7b18d1ff41c59370efb0858651d44a936c11b7b144c48fe04df3c6a3e8da');
INSERT INTO exposants (id, nom, domaine, resp_nom, resp_prenom, email, date_inscr, passwd) VALUES
    (120, 'Russia', 'Dom3', 'Parisien', 'Maxime', 'mpalladini@gmail.com', datetime('now'),
    'acc28db2beb7b42baa1cb0243d401ccb4e3fce44d7b02879a52799aadff541522d8822598b2fa664f9d5156c00c924805d75c3868bd56c2acb81d37e98e35adc');
--Valeurs test

COMMIT;
