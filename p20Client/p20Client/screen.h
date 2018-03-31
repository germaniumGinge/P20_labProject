#ifndef SCREEN_H
#define SCREEN_H

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QWidget>
//#include <QMouseEvent>
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
} posType_screen;


class screen : public QDialog
{
    Q_OBJECT

public:
    screen(QWidget *parent = 0);
    ~screen();
    //void pushBuffer();

    void setPenSize( const int& sizeIn );
    void setPenColour( const QColor& colourIn );
    void setPos(const int& x_in, const int& y_in);
    void setLastPos(const int& x_in, const int& y_in);
    void setDrawing(const bool& drawIn);
    void setClearing(const bool& clearIn);
    void setPressedChar(const char& charIn);
    void setDrawMode(const int& drawIn);
    void setKey (const QString& keyIn);

private:

protected:
    void paintEvent(QPaintEvent *event);

    QPixmap* drawScreen;
    posType_screen pos;

    //Flags
    bool drawing;
    bool clearing;

    QString key;
    int drawMode;

    //Variables
    int penWidth;
    char pressedChar;
    QColor* maincolour;

    bool check_alphanumeric(const QString& testString);

public slots:


};

#endif // SCREEN_H
