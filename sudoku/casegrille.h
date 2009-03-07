#ifndef __CASEGRILLE_H__
#define __CASEGRILLE_H__

#include <QLineEdit>

/***********************************************
 * Sous-classe de QLineEdit.  Servira pour les *
 * 81 cases de la grille de sudoku.  Le point  *
 * principal est la reimplementation de        *
 * focusInEvent et le signal recuFocus         *
 ***********************************************/

class CaseGrille : public QLineEdit {
    Q_OBJECT
    
    public:
    CaseGrille(int x = 0, int y = 0, QWidget *parent = 0);
    void setTaille(int t);
    
    int getPosX();
    void setPosX(int x);
    int getPosY();
    void setPosY(int y);
    
    /**
    * On redefinit le focusInEvent pour qu'il
    * renvoye un signal recuFocus.
    */
    void focusInEvent(QFocusEvent *event);
    
    private:
    int posX;
    int posY;
    
    signals:
    void recuFocus(int x, int y);
};

#endif //__CASEGRILLE_H__
