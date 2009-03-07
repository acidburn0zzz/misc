#include <QPaintEvent>
#include <QPainter>

#include "board.h"
#include "dalek.h"
#include "doc.h"

Board::Board(int nbCases, QWidget *parent) : QWidget(parent) {
    _nbCases = nbCases;
    _caseSize = _MIN_CASE_SIZE;
    _nbDaleks = 0;
    _grid = true;
    _doc = NULL;

    init();
}

Board::~Board() {
    delete[] _listeDaleks;
    delete _doc;
}

void Board::init() {
    setMinimumSize(_nbCases*_MIN_CASE_SIZE+1, _nbCases*_MIN_CASE_SIZE+1);

    _colBackLight = QColor(0, 0, 0, 0);
    _colBackDark = QColor(0, 0, 0, 31);

    _imgDalek = QImage("images/dalek.png");
    _imgDalekDead = QImage("images/dalek_dead.png");
    _imgDoc = QImage("images/dr.png");
    _imgDocDead = QImage("images/dr_dead.png");
}

void Board::checkImages() {
    if (_imgDalek.isNull() || _imgDalekDead.isNull() || _imgDoc.isNull() || _imgDocDead.isNull()) {
        emit imagesMissing();
    }
}

void Board::toggleGrid() {
    _grid = !_grid;
    update();
}

void Board::updateCharacters(Dalek *listeDaleks, int nbDaleks, Doc *doc) {
    _listeDaleks = listeDaleks;
    _nbDaleks = nbDaleks;
    _doc = doc;
    update();
}

void Board::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        int clickX, clickY;
        int moveX = 0, moveY = 0;
        int moveXY;
        clickX = event->x()/_caseSize;
        clickY = event->y()/_caseSize;

        if (clickX > _doc->getX()) {
            moveX = 1;
        } else if (clickX < _doc->getX()) {
            moveX = -1;
        }

        if (clickY > _doc->getY()) {
            moveY = 1;
        } else if (clickY < _doc->getY()) {
            moveY = -1;
        }

        moveXY = (moveX+2) + ((moveY*-1)+1)*3;
        emit move(moveXY);
    }
}

void Board::paintEvent(QPaintEvent * /* event */) {
    static bool firstTime = true;

    //On verifie si les images ont ete loadees la 1ere fois
    if (firstTime) {
        checkImages();
        firstTime = false;
    }

    QPainter painter(this);

    if (width() < height())
        _caseSize = (width()-1)/_nbCases;
    else
        _caseSize = (height()-1)/_nbCases;

    paintBoard(painter);
    if (_nbDaleks > 0)
        paintCharacters(painter);
}

void Board::paintBoard(QPainter &painter) {
    //Remove outline if no grid
    if (!_grid) {
        //Global outline
        painter.drawRect(0, 0, _nbCases*_caseSize, _nbCases*_caseSize);
        painter.setPen(Qt::NoPen);
    }

    for (int i=0; i<_nbCases; i++) {
        for (int j=0; j<_nbCases; j++) {
            if ((i+j)%2) {
                painter.setBrush(_colBackLight);
            } else
                painter.setBrush(_colBackDark);
            painter.drawRect(i*_caseSize, j*_caseSize, _caseSize, _caseSize);
        }
    }
}

void Board::paintCharacters(QPainter &painter) {
    painter.setPen(Qt::SolidLine);

    //Daleks
    for (int i=0; i<_nbDaleks; i++) {
        if (_listeDaleks[i].getStatut() == 0) {
            painter.drawImage(QRect((_listeDaleks[i].getX() * _caseSize)+1, (_listeDaleks[i].getY() * _caseSize)+1, _caseSize-1, _caseSize-1), _imgDalek);
        } else if (_listeDaleks[i].getStatut() == 1) {
            painter.drawImage(QRect((_listeDaleks[i].getX() * _caseSize)+1, (_listeDaleks[i].getY() * _caseSize)+1, _caseSize-1, _caseSize-1), _imgDalekDead);
        }
    }

    //Doc
    if (!_doc->getMort())
        painter.drawImage(QRect((_doc->getX() * _caseSize)+1, (_doc->getY() * _caseSize)+1, _caseSize-1, _caseSize-1), _imgDoc);
    else
        painter.drawImage(QRect((_doc->getX() * _caseSize)+1, (_doc->getY() * _caseSize)+1, _caseSize-1, _caseSize-1), _imgDocDead);
}
