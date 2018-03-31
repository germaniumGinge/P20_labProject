#ifndef CANVAS_H
#define CANVAS_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
#include <QMouseEvent>
#include <vector>
#include <map>
#include <QPainter>
#include <QDebug>
#include <QTextStream>
#include <QMessageBox>
#include <QTextCursor>
#include <QKeyEvent>
#include <QFile>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <QIODevice>
#include <QPicture>
#include <QGraphicsScene>
#include <QTimer>

#include <QRect>
#include <QDialog>
#include <QtGui>
#include <QSerialPort>

typedef struct {
    int lastX, lastY, x, y;
} posType;

typedef struct {
    int r;
    int g;
    int b;
    int a;
} rgba_type;

class canvas : public QDialog
{
    Q_OBJECT

public:
    canvas(QWidget *parent = 0);
    ~canvas();
    //void pushBuffer();

    //void setPenSize( const int& sizeIn );
    void setPenColour( const QColor& colourIn );
    void setSize(const int& newWidth, const int& newHeight);
    void setColour(const QColor& colIn);

    int getPenSize() const;
    QColor getPenColour() const;
    QPoint getPos() const;
    QPoint getLastPos() const;

    bool isDrawing() const;
    bool isClearing() const;

    char getPressedChar() const;

    int getScreenSize() const;

    rgba_type getRGBA() const;

    void setKey(const QString&);
    QString getKey() const;

    int getState() const;

public slots:

    void setText();
    void setPen();

    void setPenSize( const int& sizeIn );
    void clearAll();
    void setKey_slot(QString);

private:

protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    //void keyPressEvent(QKeyEvent * event);


    QTimer* timer;
    QPixmap* drawScreen;
    posType pos;

    //Flags
    bool drawing;
    bool clearing;

    //Variables
    int penWidth;
    char pressedChar;
    QColor* maincolour;

    QString key;

    int drawMode;
    bool check_alphanumeric(const QString&);

};

#endif // CANVAS_H
