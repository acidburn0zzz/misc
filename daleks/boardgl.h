#ifndef BOARDGL_H_INCLUDED
#define BOARDGL_H_INCLUDED

#include <QGLWidget>

class Dalek;
class Doc;

class BoardGL : public QGLWidget {
    Q_OBJECT

private:
    QColor _colRed;

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

    void init();

protected:
    void initializeGL();
    void paintGL();
    //void resizeGL(int width, int height);

public:
    BoardGL(int nbCases, QWidget *parent = 0);
    ~BoardGL();

    //QSize minimumSizeHint() const;
    //QSize sizeHint() const;

    void updateCharacters(Dalek *listeDaleks, int nbDaleks, Doc *doc);

public slots:
    void toggleGrid();

signals:
    void move(int x);
};

#endif // BOARDGL_H_INCLUDED
