#ifndef TESTSQUARE_H
#define TESTSQUARE_H

#include <QDialog>
#include <QtGui>
#include <QtWidgets>
#include <QDebug>
#include <QPainter>

class testSquare : public QDialog
{
public:
    testSquare(QWidget *parent = 0);

    void setColour(const QColor& colIn);
    QColor getColour() const;
private:
    QPainter* painter;
    QColor* colour;
    QPixmap* drawScreen;

    void paintEvent(QPaintEvent *event);
};

#endif // TESTSQUARE_H
