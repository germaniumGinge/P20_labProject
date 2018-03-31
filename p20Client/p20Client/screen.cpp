#include "screen.h"
#include <QDebug>
#define MAINSIZE 500
#define PENMODE 0
#define TEXTMODE 1

bool screen::check_alphanumeric(const QString& testString){
    QRegExp regexp("^[a-zA-Z0-9_ ]+$");
    return regexp.exactMatch(testString)? true : false;
}


screen::screen(QWidget *parent): QDialog(parent){
    //Set a fixed size for each cursor
    this->setFixedHeight(MAINSIZE);
    this->setFixedWidth(MAINSIZE);
    this->setMinimumSize(MAINSIZE, MAINSIZE);

    setMouseTracking(true);

    maincolour = new QColor(0,0,0,255);
    drawScreen = new QPixmap(MAINSIZE, MAINSIZE);
    drawScreen->fill(QColor(240,240,240,255));
    //get initial value for pos.lasts

    pos.lastX = mapFromGlobal(QCursor::pos()).x();
    pos.lastY = mapFromGlobal(QCursor::pos()).y();

    penWidth = 5;
}

screen::~screen(){
    delete maincolour;
    delete drawScreen;
}

void screen::paintEvent(QPaintEvent *event){
    QPainter* canvasPainter = new QPainter(drawScreen);

    QPen* pen = new QPen;
    pen->setColor(*maincolour);
    pen->setWidth(penWidth);
    canvasPainter->setPen(*pen);


    if (drawing && pos.lastX > 5 && pos.lastY > 5){
        switch (drawMode){
            case TEXTMODE:{
                if (!check_alphanumeric(key)){
                    break;
                }
                canvasPainter->drawText(pos.x,pos.y, key);
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

    pos.lastX = pos.x;
    pos.lastY = pos.y;


    delete pen;
    delete canvasPainter;
    delete drawPainter;
}

void screen::setPenSize( const int& sizeIn ){
    penWidth = sizeIn;
}
void screen::setPenColour( const QColor& colourIn ){
    *maincolour = colourIn;
}
void screen::setPos(const int& x_in, const int& y_in){
    pos.x = x_in;
    pos.y = y_in;
}
void screen::setLastPos(const int& x_in, const int& y_in){
    pos.lastX = x_in;
    pos.lastY = y_in;
}
void screen::setDrawing(const bool& drawIn){
    drawing = drawIn;
    update();
}
void screen::setClearing(const bool& clearIn){
    clearing = clearIn;
}
void screen::setPressedChar(const char& charIn){
    pressedChar = charIn;
}
void screen::setDrawMode(const int& drawIn){
    drawMode = drawIn;
}
void screen::setKey(const QString& keyIn){
    key = keyIn;
}
