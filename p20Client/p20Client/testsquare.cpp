#include "testsquare.h"

testSquare::testSquare(QWidget *parent): QDialog(parent){

    this->setFixedHeight(50);
    colour = new QColor(0,0,0,255);
    drawScreen = new QPixmap(this->width(), this->height());
    drawScreen->fill(*colour);

}

void testSquare::setColour(const QColor& col){

    *colour = col;
    update();
}

QColor testSquare::getColour() const{

    return *colour;
}

void testSquare::paintEvent(QPaintEvent *event){

    drawScreen->fill(*colour);
    painter = new QPainter(this);
    painter->drawPixmap(0,0,*drawScreen);
    delete painter;
}
