#include "casegrille.h"

CaseGrille::CaseGrille(int x, int y, QWidget *parent) : QLineEdit(parent) {
    posX = x;
    posY = y;
    
    setMaxLength(1);
}

void CaseGrille::setTaille(int t) {
    setMinimumSize(t, t);
    setMaximumSize(t, t);
}

int CaseGrille::getPosX() {
    return posX;
}

void CaseGrille::setPosX(int x) {
    posX = x;
}

int CaseGrille::getPosY() {
    return posY;
}

void CaseGrille::setPosY(int y) {
    posY = y;
}

void CaseGrille::focusInEvent(QFocusEvent *event) {
    QLineEdit::focusInEvent(event);
    emit recuFocus(posX, posY);
}
