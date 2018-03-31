#ifndef COMMGRID_H
#define COMMGRID_H

#include <QDialog>
#include <QtGui>
#include <QtWidgets>
#include <QDebug>
#include <QSerialPort>
#include <QMouseEvent>
#include <string>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <QIODevice>
#include <QTimer>

#include "canvas.h"
#include "screen.h"
#include "colourchooser.h"

//#include "sendcanvas.h"
//#include "receivecanvas.h"

//Parts of the dialgue box
class QLabel;

class commGrid : public QDialog
{
    Q_OBJECT

public:
    commGrid(QWidget* parent = 0);

signals:

private:
    int i;

    QString key;

    QLabel *sendHeader;
    QLabel *receiveHeader;
    //sendCanvas *sender;
    //receiveCanvas *receiver;
    canvas* sendCanvas;
    screen* receiveScreen;

    colourChooser* colourBars;

    QSerialPort* serial;
    void setupSerial();
    void serialOut();
    void serialIn();

    bool drawing;

    //void mouseMoveEvent(QMouseEvent * event);
    //void mousePressEvent(QMouseEvent *event);
    //void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent * event);

    QSpinBox* sizeEdit;

private slots:
    void timer_update();
    void transferCol();
    //void set_text();

};

#endif // COMMGRID_H
