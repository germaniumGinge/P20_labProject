#include "canvas.h"
#include <QDebug>
#define MAINSIZE 500
#define PENMODE 0
#define TEXTMODE 1
#define COLOURSCALE 40

canvas::canvas(QWidget *parent) : QDialog(parent)
{
    //Set a fixed size for each cursor
    this->setFixedHeight(MAINSIZE);
    this->setFixedWidth(MAINSIZE);
    this->setMinimumSize(MAINSIZE, MAINSIZE);

    setMouseTracking(true);
    //setupSerial();

    maincolour = new QColor(0,0,0,255);
    drawScreen = new QPixmap(MAINSIZE, MAINSIZE);
    int compColour = int(255/COLOURSCALE) * COLOURSCALE;
    drawScreen->fill(QColor(compColour,compColour,compColour,255));
    //get initial value for pos.lasts

    pos.lastX = mapFromGlobal(QCursor::pos()).x();
    pos.lastY = mapFromGlobal(QCursor::pos()).y();

    penWidth = 5;
    this->key = "Sample Text";

}
void canvas::clearAll(){
    clearing = true;
}

canvas::~canvas(){
    //serial->close();
    //delete serial;
    delete maincolour;
    delete drawScreen;
}

void canvas::mousePressEvent(QMouseEvent *event){
    drawing = 1;
    update();
}
void canvas::mouseReleaseEvent(QMouseEvent *event){
    drawing = 0;
    update();
}



void canvas::mouseMoveEvent(QMouseEvent *event){
    pos.x = mapFromGlobal(QCursor::pos()).x();
    pos.y = mapFromGlobal(QCursor::pos()).y();
    if (pos.x < 0){
        pos.x = 0;
    } else if (pos.x > this->width()){
        pos.x = this->width();
    }

    if (pos.y < 0){
        pos.y = 0;
    } else if (pos.y > this->height()){
        pos.y = this->height();
    }
    if (drawing) update();
}

void canvas::paintEvent(QPaintEvent *event){
    QPainter* canvasPainter = new QPainter(drawScreen);

    QPen* pen = new QPen;
    pen->setColor(*maincolour);
    pen->setWidth(penWidth);
    canvasPainter->setPen(*pen);


    if (drawing){
        switch (drawMode){
            case TEXTMODE:{
                if (!check_alphanumeric(key)){
                    break;
                }
                canvasPainter->drawText(pos.x,pos.y,key);
                break;
            }
            case PENMODE:{
                canvasPainter->drawLine(pos.lastX,pos.lastY, pos.x,pos.y);
                break;
            }
            default:{
                break;
            }
        };

    }

    if (clearing){
        drawScreen->fill(QColor(240,240,240,255));
        clearing = false;
    }

    QPainter* drawPainter = new QPainter(this);
    drawPainter->drawPixmap(0,0,*drawScreen);

    if (drawMode == PENMODE){
        drawPainter->drawRect(QRect(pos.x - (penWidth/2),pos.y - (penWidth/2),penWidth,penWidth));
    }
    else if (drawMode == TEXTMODE){
        drawPainter->drawText(pos.x,pos.y,key.data()[0]);
    }
    pos.lastX = pos.x;
    pos.lastY = pos.y;

    delete pen;
    delete canvasPainter;
    delete drawPainter;
}

bool canvas::check_alphanumeric(const QString& testString){
    QRegExp regexp("^[a-zA-Z0-9_ ]+$");
    return regexp.exactMatch(testString)? true : false;
}

QPoint canvas::getPos() const{
    return QPoint(pos.x,pos.y);
}

QPoint canvas::getLastPos() const{
    return QPoint(pos.lastX, pos.lastY);
}

QColor canvas::getPenColour() const{
    return *maincolour;
}

int canvas::getScreenSize() const{
    return MAINSIZE;
}

int canvas::getPenSize() const{
    return penWidth;
}
void canvas::setPenColour( const QColor& colourIn ){
    *maincolour = colourIn;
}
void canvas::setPenSize(const int &sizeIn){
    penWidth = sizeIn;
}
bool canvas::isDrawing() const{
    //qDebug() << drawing;
    return drawing;
}
bool canvas::isClearing() const{
    return clearing;
}
void canvas::setColour(const QColor& colIn){
    *maincolour = colIn;
}
void canvas::setKey(const QString& strIn){
    if (strIn == "Space"){
        key = " ";
    } else {
        key = strIn;
    }
    qDebug() << key;
}
void canvas::setKey_slot(QString strIn){
    qDebug() << strIn;
    setKey(strIn);
}

QString canvas::getKey() const{
    return this->key;
}

rgba_type canvas::getRGBA() const{
    rgba_type returner;
    returner.r = maincolour->red();
    returner.g = maincolour->green();
    returner.b = maincolour->blue();
    returner.a = maincolour->alpha();
    return returner;
}

void canvas::setSize(const int& newWidth, const int& newHeight){
    this->setFixedHeight(newHeight);
    this->setFixedWidth(newWidth);
}

void canvas::setText(){
    drawMode = TEXTMODE;
}
void canvas::setPen(){
    drawMode = PENMODE;
}
int canvas::getState() const{
    return drawMode;
}
