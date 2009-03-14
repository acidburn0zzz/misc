#include <QtGui>
#include <QtSql>

#include "login.h"
#include "sha2.h"

Login::Login(QWidget *parent) : QDialog(parent, Qt::Dialog) {
    init();
}

Login::~Login() {
    delete _lblUser;
    delete _lblPass;
    delete _txtUser;
    delete _txtPass;
    delete _btnOk;
    delete _btnQuit;
    
    delete _layUser;
    delete _layBoutons;
    delete _centralLayout;
}

void Login::init() {
    this->setWindowTitle(tr("Login"));
    
    //Affichage
    _lblUser = new QLabel(tr("Username: "));
    _lblPass = new QLabel(tr("Password: "));
    _txtUser = new QLineEdit();
    _txtPass = new QLineEdit();
    _txtPass->setEchoMode(QLineEdit::Password);
    
    _layUser = new QGridLayout();
    _layUser->addWidget(_lblUser, 0, 0);
    _layUser->addWidget(_txtUser, 0, 1);
    _layUser->addWidget(_lblPass, 1, 0);
    _layUser->addWidget(_txtPass, 1, 1);
    
    //Ajout des boutons
    _btnOk = new QPushButton(tr("&Ok"));
    _btnQuit = new QPushButton(tr("&Quit"));
    connect(_btnOk, SIGNAL(clicked()), this, SLOT(checkUser()));
    connect(_btnQuit, SIGNAL(clicked()), this, SLOT(reject()));
    
    _layBoutons = new QHBoxLayout();
    _layBoutons->addWidget(_btnOk);
    _layBoutons->addWidget(_btnQuit);
    
    _centralLayout = new QVBoxLayout();
    _centralLayout->addLayout(_layUser);
    _centralLayout->addLayout(_layBoutons);
    
    this->setLayout(_centralLayout);
}

void Login::checkUser() {
    sha256_ctx ctx;
	unsigned char hval[SHA256_DIGEST_SIZE], *data;
    char passwd_hash[65];
    int len;
    static int tries=0;
    
    len = _txtPass->text().length();
    data = new unsigned char[len];
    
    strncpy((char*)data, _txtPass->text().toStdString().c_str(), len);
    
    sha256_begin(&ctx);
    sha256_hash(data, len, &ctx);
    sha256_end(hval, &ctx);
    
    delete data;
    for (int i=0; i<SHA256_DIGEST_SIZE; i++)
        sprintf(passwd_hash+2*i, "%.2x", hval[i]);
    passwd_hash[64] = '\0';
    
    #ifdef DEBUG
    qDebug() << passwd_hash;
    #endif
    
    //Verification avec la base de donnees
    QSqlQuery q;
    q.exec("SELECT * FROM users");
    while (q.next()) {
        if (q.value(1).toString() == _txtUser->text()) {
            if (q.value(3).toString() == "locked") {
                QMessageBox::warning(this, tr("Account locked"), tr("Your account was locked.\nPlease contact the administrator."));
                this->reject();
                return;
            }
            if (q.value(2).toString() == QString(passwd_hash)) {
                this->accept();
                return;
            }
        } else {
            _txtPass->clear();
        }
    }
    
    tries++;
    
    #ifdef DEBUG
    qDebug() << "Tries:" << tries;
    #endif
    if (tries == 5) {
        q.prepare("UPDATE users SET account_type='locked' WHERE username=?");
        q.addBindValue(QVariant(_txtUser->text()));
        q.exec();
        QMessageBox::warning(this, tr("Account locked"), tr("Your account was locked.\nPlease contact the administrator."));
        this->reject();
        return;
    }
}
