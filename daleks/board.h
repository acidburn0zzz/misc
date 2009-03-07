#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include <QWidget>

class Dalek;
class Doc;

class Board : public QWidget {
    Q_OBJECT

private:
    void paintBoard(QPainter &painter);
    void paintCharacters(QPainter &painter);

    QColor _colBackLight, _colBackDark;

    QImage _imgDalek;
    QImage _imgDalekDead;
    QImage _imgDoc;
    QImage _imgDocDead;

    int _nbCases;
    int _nbDaleks;
    bool _grid;
    const static int _MIN_CASE_SIZE = 20;
    int _caseSize;

    Dalek *_listeDaleks;
    Doc *_doc;

    void checkImages();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

public:
    Board(int nbCases, QWidget *parent = 0);
    ~Board();

    void init();
    void updateCharacters(Dalek *listeDaleks, int nbDaleks, Doc *doc);

public slots:
    void toggleGrid();

signals:
    void move(int x);
    void imagesMissing();
};

#endif // BOARD_H_INCLUDED
