BEGIN TRANSACTION;

DROP TABLE IF EXISTS p_type;
DROP TABLE IF EXISTS p_suivi_atelier;
DROP TABLE IF EXISTS p_region;
DROP TABLE IF EXISTS p_local;
DROP TABLE IF EXISTS p_inscription_archive;
DROP TABLE IF EXISTS p_inscription;
DROP TABLE IF EXISTS p_exposant_archive;
DROP TABLE IF EXISTS p_exposant;
DROP TABLE IF EXISTS p_expo;
DROP TABLE IF EXISTS p_evaluation;
DROP TABLE IF EXISTS p_ecole;
DROP TABLE IF EXISTS p_critere;
DROP TABLE IF EXISTS p_categorie;
DROP TABLE IF EXISTS p_auditeur;
DROP TABLE IF EXISTS p_atelier;

CREATE TABLE p_type (
    notype INTEGER PRIMARY KEY AUTOINCREMENT,
    nomtype TEXT NOT NULL
);

CREATE TABLE p_suivi_atelier (
    no_suivi_atel INTEGER PRIMARY KEY AUTOINCREMENT,
    no_atel INTEGER NOT NULL,
    no_ancien_loc TEXT NOT NULL
);

CREATE TABLE p_region (
    noregion INTEGER PRIMARY KEY AUTOINCREMENT,
    nomregion TEXT NOT NULL
);

CREATE TABLE p_local (
    idlocal INTEGER PRIMARY KEY AUTOINCREMENT,
    nolocal TEXT,
    capacite INTEGER NOT NULL
);

CREATE TABLE p_inscription_archive (
    no_ins_arch INTEGER PRIMARY KEY AUTOINCREMENT,
    noauditeur INTEGER NOT NULL,
    noatel INTEGER NOT NULL,
    dateinscription TEXT,
    mode_paiement TEXT,
    no_carte TEXT,
    no_cheque INTEGER,
    date_expire TEXT,
    acquitter INTEGER,
    confirmationenvoyee INTEGER,
    evenement TEXT,
    datemodif TEXT
);

CREATE TABLE p_inscription (
    noauditeur INTEGER NOT NULL,
    noatel INTEGER NOT NULL,
    dateinscription TEXT,
    mode_paiement TEXT,
    no_carte TEXT,
    no_cheque INTEGER,
    date_expire TEXT,
    acquitter INTEGER,
    confirmationenvoyee INTEGER,
    
    CONSTRAINT p_inscription_pk UNIQUE (noauditeur, noatel),
    CONSTRAINT p_inscription_auditeur_fk FOREIGN KEY (noauditeur) REFERENCES p_auditeur (noauditeur),
    CONSTRAINT p_inscription_atelier_fk FOREIGN KEY (noatel) REFERENCES p_atelier (noatel)
);

CREATE TABLE p_exposant_archive (
    no_exp_arch INTEGER PRIMARY KEY AUTOINCREMENT,
    noexposant INTEGER NOT NULL,
    nom TEXT,
    prenom TEXT,
    nocoequipier INTEGER,
    noecole INTEGER,
    rue TEXT,
    ville TEXT,
    code_postal TEXT,
    telephone INTEGER,
    courriel TEXT,
    creepar TEXT,
    datesupression TEXT
);

CREATE TABLE p_exposant (
    noexposant INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT,
    prenom TEXT,
    nocoequipier INTEGER,
    noecole INTEGER,
    rue TEXT,
    ville TEXT,
    code_postal TEXT,
    telephone INTEGER,
    courriel TEXT,
    creepar TEXT,
    
    CONSTRAINT p_exposant_ecole_fk FOREIGN KEY (noecole) REFERENCES p_ecole (noecole)
);

CREATE TABLE p_expo (
    noexpo INTEGER PRIMARY KEY AUTOINCREMENT,
    text_debut TEXT,
    text_fin TEXT,
    nomhotel TEXT,
    rue TEXT,
    ville TEXT,
    code_postal TEXT,
    telephone INTEGER,
    courriel TEXT,
    creepar TEXT
);

CREATE TABLE p_evaluation (
    noeval INTEGER PRIMARY KEY AUTOINCREMENT,
    juge INTEGER NOT NULL,
    cote INTEGER,
    critere INTEGER NOT NULL,
    noatel INTEGER NOT NULL,
    
    CONSTRAINT p_evaluation_p_atelier_fk FOREIGN KEY (noatel) REFERENCES p_atelier (noatel),
    CONSTRAINT p_evaluation_p_auditeur_fk FOREIGN KEY (juge) REFERENCES p_auditeur (noauditeur),
    CONSTRAINT p_evaluation_p_critere_fk FOREIGN KEY (critere) REFERENCES p_critere (nocritere)
);

CREATE TABLE p_ecole (
    noecole INTEGER PRIMARY KEY AUTOINCREMENT,
    nomecole TEXT,
    noregion INTEGER,
    tuteur TEXT,
    rue TEXT,
    ville TEXT,
    code_postal TEXT,
    telephone INTEGER,
    courriel TEXT,
    
    CONSTRAINT p_ecole_region_fk FOREIGN KEY (noregion) REFERENCES P_REGION (noregion)
);

CREATE TABLE p_critere (
    nocritere INTEGER PRIMARY KEY AUTOINCREMENT,
    nomcritere TEXT
);

CREATE TABLE p_categorie (
    nocategorie INTEGER PRIMARY KEY AUTOINCREMENT,
    nom TEXT,
    description TEXT
);

CREATE TABLE p_auditeur (
    noauditeur INTEGER PRIMARY KEY AUTOINCREMENT,
    codeauditeur TEXT,
    motdepasse TEXT,
    nom TEXT,
    prenom TEXT,
    juge INTEGER,
    rue TEXT,
    ville TEXT,
    code_postal TEXT,
    noregion INTEGER,
    telephone INTEGER,
    courriel TEXT,
    statut TEXT,
    
    --Pas certain
    CONSTRAINT p_atelier_p_region_fk FOREIGN KEY (noregion) REFERENCES p_region (noregion),
    
    CONSTRAINT statut CHECK (statut = 'R' OR statut = 'r' OR statut = 'E' OR statut = 'e')
);

CREATE TABLE p_atelier (
    noatel INTEGER PRIMARY KEY AUTOINCREMENT,
    titre TEXT,
    langue TEXT,
    acetate_elec INTEGER,
    portable INTEGER,
    retro INTEGER,
    duree INTEGER,
    nbmaximum INTEGER,
    nolocal TEXT,
    dateatel TEXT, --Au format 'YYYY-MM-DD HH:MM:SS'
    creepar TEXT,
    datecreation TEXT,
    coutetudiant INTEGER,
    coutregulier INTEGER,
    nocategorie INTEGER NOT NULL,
    notype INTEGER NOT NULL,
    noexposant INTEGER NOT NULL,
    
    CONSTRAINT p_atelier_local_fk FOREIGN KEY (nolocal) REFERENCES p_local (idlocal),
    CONSTRAINT p_atelier_p_categorie_fk FOREIGN KEY (nocategorie) REFERENCES p_categorie (nocategorie),
    CONSTRAINT p_atelier_p_exposant_fk FOREIGN KEY (noexposant) REFERENCES p_exposant (noexposant),
    CONSTRAINT p_atelier_p_type_fk FOREIGN KEY (notype) REFERENCES p_type (notype),
    
    CONSTRAINT acetate_elec CHECK (acetate_elec = 1 OR acetate_elec = 0),
    CONSTRAINT duree CHECK (duree IN (30, 45, 60, 90)),
    CONSTRAINT langue CHECK (langue = 'A' OR langue = 'F' OR langue = 'a' OR langue = 'f'),
    CONSTRAINT portable CHECK (portable = 1 OR portable = 0)
);

INSERT INTO p_type (nomtype) VALUES ('Séminaire');
INSERT INTO p_type (nomtype) VALUES ('Plénière');
INSERT INTO p_type (nomtype) VALUES ('Interactif');

INSERT INTO p_region (nomregion) VALUES ('Montréal');
INSERT INTO p_region (nomregion) VALUES ('Laval');
INSERT INTO p_region (nomregion) VALUES ('Québec');
INSERT INTO p_region (nomregion) VALUES ('Sherbrooke');
INSERT INTO p_region (nomregion) VALUES ('Abitibi');

INSERT INTO p_local (nolocal, capacite) VALUES ('A5.30', 50);
INSERT INTO p_local (nolocal, capacite)  VALUES ('A5.31', 45);
INSERT INTO p_local (nolocal, capacite)  VALUES ('A5.32', 40);
INSERT INTO p_local (nolocal, capacite)  VALUES ('A5.33', 35);
INSERT INTO p_local (nolocal, capacite)  VALUES ('A5.34', 30);

INSERT INTO p_exposant (nom, prenom) VALUES ('1', 'Exposant');
INSERT INTO p_exposant (nom, prenom) VALUES ('2', 'Exposant');
INSERT INTO p_exposant (nom, prenom) VALUES ('3', 'Exposant');
INSERT INTO p_exposant (nom, prenom) VALUES ('4', 'Exposant');
INSERT INTO p_exposant (nom, prenom) VALUES ('5', 'Exposant');

INSERT INTO p_inscription (noauditeur, noatel, dateinscription, mode_paiement, no_carte, no_cheque, date_expire, acquitter, confirmationenvoyee)
    VALUES (1, 1, '25-12-0000', 'Cheque', '', 12, "25-12-0002", 1, 1);
INSERT INTO p_inscription (noauditeur, noatel, dateinscription, mode_paiement, no_carte, no_cheque, date_expire, acquitter, confirmationenvoyee)
    VALUES (1, 2, '25-12-0000', 'Cheque', '', 12, "25-12-0002", 1, 1);
INSERT INTO p_inscription (noauditeur, noatel, dateinscription, mode_paiement, no_carte, no_cheque, date_expire, acquitter, confirmationenvoyee)
    VALUES (1, 3, '25-12-0000', 'Cheque', '', 12, "25-12-0002", 1, 1);
INSERT INTO p_inscription (noauditeur, noatel, dateinscription, mode_paiement, no_carte, no_cheque, date_expire, acquitter, confirmationenvoyee)
    VALUES (2, 1, '25-12-0000', 'Cheque', '', 12, "25-12-0002", 1, 1);
INSERT INTO p_inscription (noauditeur, noatel, dateinscription, mode_paiement, no_carte, no_cheque, date_expire, acquitter, confirmationenvoyee)
    VALUES (1, 5, '25-12-0000', 'Cheque', '', 12, "25-12-0002", 1, 1);
    
INSERT INTO p_auditeur (codeauditeur, motdepasse, nom, prenom, juge, rue, ville, code_postal, noregion, telephone, courriel, statut)
    VALUES ('1', 'patate', 'Wong', 'Elvis', 0, 'Taillon', 'Montréal', 'H1L 4K5', 1, 5143553179, 'patate@patate.com', 'E');
INSERT INTO p_auditeur (codeauditeur, motdepasse, nom, prenom, juge, rue, ville, code_postal, noregion, telephone, courriel, statut)
    VALUES ('1', 'patate', 'Presley', 'Elvis', 0, 'Taillon', 'Montréal', 'H1L 4K5', 1, 5143553179, 'patate@patate.com', 'e');
INSERT INTO p_auditeur (codeauditeur, motdepasse, nom, prenom, juge, rue, ville, code_postal, noregion, telephone, courriel, statut)
    VALUES ('1', 'patate', 'Gratton', 'Elvis', 1, 'Taillon', 'Montréal', 'H1L 4K5', 1, 5143553179, 'patate@patate.com', 'R');
INSERT INTO p_auditeur (codeauditeur, motdepasse, nom, prenom, juge, rue, ville, code_postal, noregion, telephone, courriel, statut)
    VALUES ('1', 'patate', 'Gingras', 'Elvis', 0, 'Taillon', 'Montréal', 'H1L 4K5', 1, 5143553179, 'patate@patate.com', 'r');
--INSERT INTO p_atelier (titre, langue, acetate_elec, portable, duree, nbmaximum, nolocal, dateatel, creepar, datecreation, coutetudiant, coutregulier, nocategorie, notype, noexposant)
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);

INSERT INTO p_categorie (nom ,description) VALUES ("Chimie Organique", "Chimie Organique");
INSERT INTO p_categorie (nom ,description) VALUES ("Sciences Humaines", "Sciences Humaines");
INSERT INTO p_categorie (nom ,description) VALUES ("Sciences de la Nature", "Sciences de la Nature");



INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Le Biocaburant', 'F', 1, 60, 25, 1, '2001-12-25 10:00:00', 1, 1, 1);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('La Biosphère', 'F', 1, 60, 25, 2, '2002-12-25 11:00:00', 2, 2, 2);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Database', 'A', 1, 60, 25, 3, '2003-12-25 12:00:00', 3, 3, 3);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('Java', 'F', 1, 60, 25, 4, '2004-12-25 13:00:00', 1, 1, 4);
INSERT INTO p_atelier (titre, langue, acetate_elec, duree, nbmaximum, nolocal, dateatel, nocategorie, notype, noexposant)
    VALUES ('C++', 'F', 1, 60, 25, 5, '2009-04-10 14:00:00', 2, 2, 5);

COMMIT;