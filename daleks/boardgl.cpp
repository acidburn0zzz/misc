#include <QtOpenGL>
#include <QtGui>

#include <cmath>

#include "boardgl.h"
#include "dalek.h"
#include "doc.h"

BoardGL::BoardGL(int nbCases, QWidget *parent) : QGLWidget(parent) {
    _nbCases = nbCases;
    _caseSize = _MIN_CASE_SIZE;
    _nbDaleks = 0;
    _grid = true;
    _doc = NULL;

    init();
}

BoardGL::~BoardGL() {
    delete[] _listeDaleks;
    delete _doc;
}

void BoardGL::init() {
    setMinimumSize(_nbCases*_caseSize+1, _nbCases*_caseSize+1);

    _colRed = QColor(255, 0, 0);
}

void BoardGL::initializeGL() {
    qglClearColor(_colRed.dark());
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void BoardGL::paintGL() {

}

void BoardGL::toggleGrid() {
    _grid = !_grid;
    update();
}

void BoardGL::updateCharacters(Dalek *listeDaleks, int nbDaleks, Doc *doc) {
    _listeDaleks = listeDaleks;
    _nbDaleks = nbDaleks;
    _doc = doc;
    update();
}
