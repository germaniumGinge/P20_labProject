#ifndef COLOURCHOOSER_H
#define COLOURCHOOSER_H

#include <QDialog>
#include <QtGui>
#include <QtWidgets>
#include <QDebug>
#include <QSlider>

#include "testsquare.h"

class QSlider;
class QPushButton;

class colourChooser : public QDialog
{
    Q_OBJECT
public:
    colourChooser(QWidget *parent = 0);
    ~colourChooser();
    QColor getColor() const;
    void updateCurrent();
signals:

public slots:

private:
    QSlider* red;
    QSlider* green;
    QSlider* blue;

    testSquare* currRect;
    testSquare* newRect;

    QColor* mainColour;

private slots:
    void setRed(int);
    void setBlue(int);
    void setGreen(int);

   // QPushButton* setButton;
};

#endif // COLOURCHOOSER_H
