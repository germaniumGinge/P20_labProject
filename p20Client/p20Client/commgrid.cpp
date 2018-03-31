#include "commgrid.h"
#include <QTimer>

#define COLOURSCALE 1
#define PIXELSCALE 1

commGrid::commGrid(QWidget *parent) : QDialog(parent)
{
    sendHeader = new QLabel("Your drawing");
    receiveHeader = new QLabel("Recieved drawing");

    QFont* headerFont = new QFont;
    headerFont->setBold(true);
    headerFont->setPixelSize(13);

    //Configuring the canvas' title
    sendHeader->setFont(*headerFont);
    sendHeader->setFixedHeight(20);
    sendHeader->setAlignment(Qt::AlignCenter);

    receiveHeader->setFont(*headerFont);
    receiveHeader->setFixedHeight(20);
    receiveHeader->setAlignment(Qt::AlignCenter);

    //setup main screen
    sendCanvas = new canvas;
    receiveScreen = new screen;
    colourBars = new colourChooser;

    QPushButton* setColour = new QPushButton("Set Colour");
    QPushButton* setText = new QPushButton("Type text");
    QPushButton* setPen = new QPushButton("Draw");
    QPushButton* clearButton = new QPushButton("Clear All");

    QLineEdit* setKey = new QLineEdit("Sample Text");

    connect(setColour, SIGNAL(released()), this, SLOT(transferCol()));
    connect(setText,   SIGNAL(released()), sendCanvas, SLOT(setText()));
    connect(setPen,   SIGNAL(released()), sendCanvas, SLOT(setPen()));
    connect(setKey, SIGNAL(textEdited(QString)), sendCanvas, SLOT(setKey_slot(QString)));
    connect(clearButton,SIGNAL(released()), sendCanvas, SLOT(clearAll()));

    //sizeSlider = new QSlider(Qt::Horizontal);
    //sizeSlider->setMinimum(0);
    //sizeSlider->setMaximum(50);

    //connect(sizeSlider, SIGNAL(valueChanged(int)), this, SLOT(setPenSize(int)));

    sizeEdit = new QSpinBox();
    sizeEdit->setValue(5);
    connect(sizeEdit, SIGNAL(valueChanged(int)), sendCanvas, SLOT(setPenSize(int)));

    //Setting up layouts
    QVBoxLayout* sendV = new QVBoxLayout;
    QVBoxLayout* receiveV = new QVBoxLayout;
    QVBoxLayout* toolV = new QVBoxLayout;
    QHBoxLayout* mainView = new QHBoxLayout;
    QVBoxLayout* sizeView = new QVBoxLayout;

    //Setting up widgets
        //Sender
    sendV->addWidget(sendHeader);
    sendV->addWidget(sendCanvas);
        //Receiver
    receiveV->addWidget(receiveHeader);
    receiveV->addWidget(receiveScreen);
        //Toolbar

    QLabel* sizeSettings = new QLabel("Set size:");
    sizeSettings->setFont(*headerFont);
    sizeSettings->setFixedHeight(20);

    sizeView->addWidget(sizeSettings);
    sizeView->addWidget(sizeEdit);

    QLabel* colourSettings = new QLabel("Colour settings");
    colourSettings->setFont(*headerFont);
    colourSettings->setFixedHeight(20);

    toolV->addWidget(colourSettings);
    toolV->addWidget(colourBars);
    toolV->addWidget(setColour);
    toolV->addLayout(sizeView);
    toolV->addWidget(setPen);
    toolV->addWidget(setKey);
    toolV->addWidget(setText);
    toolV->addWidget(clearButton);

    //Slotting widgets into main view
    mainView->addLayout(toolV);
    mainView->addLayout(sendV);
    mainView->addLayout(receiveV);
    this->setLayout(mainView);

    setupSerial();

    //Configure timer to send serial data every <n> milliseconds
    QTimer* timer = new QTimer(this);
    connect (timer, SIGNAL(timeout()), this, SLOT(timer_update()));
    timer->start(5);

}

void commGrid::transferCol(){
    sendCanvas->setColour(colourBars->getColor());
    colourBars->updateCurrent();
}

void commGrid::setupSerial(){
    serial = new QSerialPort();
    //Configure serial port to have
    //  * 8 data bits
    //  * No parity bits
    //  * One stop bit
    //  * No flow control
    serial->setPortName("/dev/ttyUSB0");
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (!(serial->open(QIODevice::ReadWrite))){
        QMessageBox::critical(this, tr("Error"), serial->errorString());
    } else {
        qDebug() << "Serial port configured.";
    }
}

void commGrid::serialOut(){ //Takes data from canvas and serializes it
    std::string bufferOut = "";
    //test for any differences using the following statics:
    static QColor colorBuff, last_colorBuff;
    static QString stringBuff, last_stringBuff;
    static int widthBuff, last_widthBuff;
    static int stateBuff, last_stateBuff;
    static QPoint coordBuffer, last_coordBuffer;

    //Get canvas data
    colorBuff = sendCanvas->getPenColour();
    stringBuff = sendCanvas->getKey();
    widthBuff = sendCanvas->getPenSize();
    stateBuff = sendCanvas->getState();
    coordBuffer = sendCanvas->getPos();

    /* Note: The following has to be put in the frame FIRST because strings
     *       are going to have multiple characters which might interfere with
     *       the frame otherwise. The receiver takes this string, interprets
     *       it, then throws it away so that the rest of the frame is not
     *       interfered with.
     */
    if (stringBuff != last_stringBuff){ //String has varied
        bufferOut.append("z" + (sendCanvas->getKey()).toStdString() + "Z");
    }

    //Boolean variables
    if (sendCanvas->isClearing()){
        bufferOut.append("C");
    }
    if (sendCanvas->isDrawing()){ //Drawing, therefore send X and Y
        bufferOut.append("D");
    }

    //X and Y mouse data
    if (coordBuffer != last_coordBuffer){ //Coords have varied
        bufferOut.append("x" + std::to_string(sendCanvas->getPos().x()/PIXELSCALE));
        bufferOut.append("y" + std::to_string(sendCanvas->getPos().y()/PIXELSCALE) + "Y");
    }

    //Integer variables
    if (widthBuff != last_widthBuff){ //Width has varied
        bufferOut.append("w" + std::to_string(sendCanvas->getPenSize()) + "W");
    }
    if (stateBuff != last_stateBuff){ //String has varied
        bufferOut.append("s" + std::to_string(sendCanvas->getState()) + "S");
    }

    //RGB data
    if (colorBuff != last_colorBuff){ // Colour has varied
        bufferOut.append("r" + std::to_string(sendCanvas->getRGBA().r/COLOURSCALE));
        bufferOut.append("g" + std::to_string(sendCanvas->getRGBA().g/COLOURSCALE));
        bufferOut.append("b" + std::to_string(sendCanvas->getRGBA().b/COLOURSCALE) + "B");
    }

    bufferOut.append("E\n"); //Default blank data frame - receiver ignores strings of this exact type
    //qDebug() << QString(bufferOut.c_str());
    serial->write(bufferOut.c_str());

    //Update static variables
    last_colorBuff = colorBuff;
    last_stringBuff = stringBuff;
    last_widthBuff = widthBuff;
    last_stateBuff = stateBuff;
    last_coordBuffer = coordBuffer;

    update();
    //Don't overload the canvas with constant data, add time between sends
    //This specifc time may be tweaked when I start using GPIOs
    usleep(10000);
}

void commGrid::serialIn(){ //Deserializes the data frame and extracts any useful info
   /* Two key functions that are needed to understand this code are string.mid(a,b); and string.indexOf(x);
    * the mid function gets a substring which starts with the element a and ends with element b.
    * The indexof function searches for the first instance of a particular substring and returns its place in the string.
    */
    QString QBuf;
    int getX, getY, /* getLastX, getLastY, */ getRed, getGreen, getBlue/*, getAlpha*/, getState, getWidth;
    QString getString;
    if (serial->isOpen() && serial->isWritable()){ //Check if input buffer is ready to recieve
        QBuf = QString(serial->readLine());
        //qDebug() << QBuf;
        if (!QBuf.endsWith("E\n")){ //Corrupt data frame :(
            usleep(20000); //Wait for next frame, hoping the corruption has ended
            return;
        }
        if (QBuf == "E\n"){ //Blank data frame, do nothing.
            return;
        }

        //INPUT STRING
        //If there is a string, seperate that first or else there will be ambiguity
        if (QBuf.contains("z") && (QBuf.contains("Z"))){
            getString = (QBuf.mid(QBuf.indexOf("z")+1, ((QBuf.lastIndexOf("Z"))-(QBuf.indexOf("z")))-1));
            QBuf = QBuf.right(QBuf.lastIndexOf("Z")+1) + QBuf.left(QBuf.indexOf("z")-1); //Removes string to remove ambiguity
            receiveScreen->setKey(getString);
            //qDebug() << "Received key: " << getString;
        }
        //From now on, don't contain "z" or "Z" in your data frame.
        //Now, test if the clear button has been pressed.
        if (QBuf.contains("C")){
            receiveScreen->setClearing(true);
        } else{
            receiveScreen->setClearing(false);
        }
        //Test if the sender is drawing or not
        if (QBuf.contains("D")){
            receiveScreen->setDrawing(true);
        } else{
            receiveScreen->setDrawing(false);
        }
        if (QBuf.contains("w")){ //Update to pen width
            getWidth = (QBuf.mid(QBuf.indexOf("w")+1, ((QBuf.indexOf("W"))-(QBuf.indexOf("w")))-1)).toInt();
            receiveScreen->setPenSize(getWidth);
        }
        if (QBuf.contains("s")){ //Update to draw state
            getState = (QBuf.mid(QBuf.indexOf("s")+1, ((QBuf.lastIndexOf("S"))-(QBuf.indexOf("s")))-1)).toInt();
            receiveScreen->setDrawMode(getState);
        }
        if ((QBuf.contains("x")) && (QBuf.contains("Y"))){ //Update to coordinates
            getX = (QBuf.mid(QBuf.indexOf("x")+1, ((QBuf.indexOf("y"))-(QBuf.indexOf("x")))-1)).toInt();
            getY = (QBuf.mid(QBuf.indexOf("y")+1, ((QBuf.indexOf("Y"))-(QBuf.indexOf("y")))-1)).toInt();
            if (!((getX < 5 && getY < 5) || (getX > this->width() && getY > this->height()))){
                receiveScreen->setPos(getX * PIXELSCALE,getY * PIXELSCALE);
            }
        }

        if (QBuf.contains("r") && QBuf.contains("g") && QBuf.contains("b")){ //Update to colour
            getRed   = (QBuf.mid(QBuf.indexOf("r")+1, ((QBuf.indexOf("g"))-(QBuf.indexOf("r")))-1)).toInt();
            getGreen = (QBuf.mid(QBuf.indexOf("g")+1, ((QBuf.indexOf("b"))-(QBuf.indexOf("g")))-1)).toInt();
            getBlue  = (QBuf.mid(QBuf.indexOf("b")+1, ((QBuf.indexOf("B"))-(QBuf.indexOf("b")))-1)).toInt();
            receiveScreen->setPenColour(QColor(COLOURSCALE*getRed,COLOURSCALE*getGreen,COLOURSCALE*getBlue,255));
        }
    }
}

void commGrid::timer_update(){
    //qDebug() << i;
    serialOut();
    serialIn();
    //i++;
}

void commGrid::keyPressEvent(QKeyEvent * event){
    key= QKeySequence(event->key()).toString();
    //qDebug() << "Key pressed: " << key;
    sendCanvas->setKey(key);
   // update();
}
