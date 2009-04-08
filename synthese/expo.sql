BEGIN TRANSACTION;

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
    nolocal TEXT PRIMARY KEY,
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
    text_expire TEXT,
    acquitter INTEGER,
    confirmationenvoyee INTEGER,
    
    CONSTRAINT p_inscription_pk UNIQUE (noauditeur, noatel)
    CONSTRAINT p_inscription_atelier_fk FOREIGN KEY (noatel) REFERENCES p_atelier (noatel),
    CONSTRAINT p_inscription_auditeur_fk FOREIGN KEY (noauditeur) REFERENCES p_auditeur (noauditeur)
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
    
    CONSTRAINT statut CHECK (statut = 'R' OR statut = 'r' OR statut = 'E' OR statut = 'e')
);

CREATE TABLE p_atelier (
    noatel INTEGER PRIMARY KEY AUTOINCREMENT,
    titre TEXT,
    langue TEXT,
    acetate_elec INTEGER,
    portable INTEGER,
    duree INTEGER,
    nbmaximum INTEGER,
    nolocal TEXT,
    dateatel TEXT,
    creepar TEXT,
    datecreation TEXT,
    coutetudiant INTEGER,
    coutregulier INTEGER,
    nocategorie INTEGER NOT NULL,
    notype INTEGER NOT NULL,
    noexposant INTEGER NOT NULL,
    
    CONSTRAINT p_atelier_local_fk FOREIGN KEY (nolocal) REFERENCES p_local (nolocal),
    CONSTRAINT p_atelier_p_categorie_fk FOREIGN KEY (nocategorie) REFERENCES p_categorie (nocategorie),
    CONSTRAINT p_atelier_p_exposant_fk FOREIGN KEY (noexposant) REFERENCES p_exposant (noexposant),
    CONSTRAINT p_atelier_p_type_fk FOREIGN KEY (notype) REFERENCES p_type (notype),
    
    CONSTRAINT acetate_elec CHECK (acetate_elec = 1 OR acetate_elec = 0),
    CONSTRAINT duree CHECK (duree IN (30, 45, 60, 90)),
    CONSTRAINT langue CHECK (langue = 'A' OR langue = 'F' OR langue = 'a' OR langue = 'f'),
    CONSTRAINT portable CHECK (portable = 1 OR portable = 0)
);

COMMIT;