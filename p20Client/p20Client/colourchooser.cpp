#include "colourchooser.h"

colourChooser::colourChooser(QWidget *parent) : QDialog(parent){

    QSizePolicy sizePol;
    sizePol.setVerticalPolicy(QSizePolicy::Minimum);

    red = new QSlider(Qt::Horizontal);
    green = new QSlider(Qt::Horizontal);
    blue = new QSlider(Qt::Horizontal);

    red->setMinimum(0);
    red->setMaximum(240);

    green->setMinimum(0);
    green->setMaximum(240);

    blue->setMinimum(0);
    blue->setMaximum(240);

    /*
    red->setSizePolicy(sizePol);
    green->setSizePolicy(sizePol);
    blue->setSizePolicy(sizePol);
    */
    red->setFixedHeight(15);
    blue->setFixedHeight(15);
    green->setFixedHeight(15);

    //testRect = new QRect(0, 0, red->width(), red->height());
    //currRect = new QRect(0, 0, red->width(), red->height());

    QVBoxLayout* arrange = new QVBoxLayout;

    QLabel* redLabel = new QLabel("Red");
    QLabel* greenLabel = new QLabel("Green");
    QLabel* blueLabel = new QLabel("Blue");

    redLabel->setFixedHeight(50);
    greenLabel->setFixedHeight(50);
    blueLabel->setFixedHeight(50);

    arrange->addWidget(redLabel);
    arrange->addWidget(red);

    arrange->addWidget(greenLabel);
    arrange->addWidget(green);

    arrange->addWidget(blueLabel);
    arrange->addWidget(blue);

    this->setSizePolicy(sizePol);
    this->setLayout(arrange);

    mainColour = new QColor(0,0,0,255);

    connect(red,   SIGNAL(valueChanged(int)), this, SLOT(setRed(int)));
    connect(green, SIGNAL(valueChanged(int)), this, SLOT(setGreen(int)));
    connect(blue,  SIGNAL(valueChanged(int)), this, SLOT(setBlue(int)));

    currRect = new testSquare;
    newRect = new testSquare;

    arrange->addWidget(newRect);
    arrange->addWidget(currRect);
}

void colourChooser::setRed(int inputCol){
    mainColour->setRed(inputCol);
    newRect->setColour(*mainColour);
    qDebug() << "New red: " << inputCol;
}
void colourChooser::setBlue(int inputCol){
    mainColour->setBlue(inputCol);
    newRect->setColour(*mainColour);
    qDebug() << "New blue: " << inputCol;
}
void colourChooser::setGreen(int inputCol){
    mainColour->setGreen(inputCol);
    newRect->setColour(*mainColour);
    qDebug() << "New green: " << inputCol;
}

QColor colourChooser::getColor() const{
    return *mainColour;
}

void colourChooser::updateCurrent(){
    currRect->setColour(*mainColour);
}

colourChooser::~colourChooser(){
    delete red;
    delete green;
    delete blue;

    delete currRect;
    delete newRect;
}
