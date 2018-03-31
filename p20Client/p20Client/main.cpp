
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <pthread.h>

#include "commgrid.h"
/*
void* worker(void* thread_id){
    long tid = (long)thread_id;
    qDebug() << "Worker thread: "<<tid<<" started.";
    pthread_exit(NULL);
}
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget* mainWindow = new QWidget();
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWindow);

    //Configuring title settings
    QLabel* title = new QLabel("P20 Project: Whiteboard Chat");
    QFont* titleFont = new QFont;
    titleFont->setBold(true);
    titleFont->setPixelSize(15);
    title->setFont(*titleFont);
    title->setFixedHeight(30);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    //Configuring painted sudoku solver
    commGrid* sendReceiveGrid = new commGrid;

    //sendReceiveGrid->setMinimumSize(1000,500);
    mainLayout->addWidget(sendReceiveGrid);

    //Configuring button toolbar
    //toolbar* buttonBar = new toolbar;
    //mainLayout->addWidget(buttonBar);

    mainWindow->setWindowTitle("P20 Project: Whiteboard Chat");
    mainWindow->show();

    //QCursor cursor(Qt::BlankCursor);
    //QApplication::setOverrideCursor(cursor);
    //QApplication::changeOverrideCursor(cursor);

    QPalette pal;
    pal.setColor(QPalette::Background, Qt::lightGray);
    mainWindow->setAutoFillBackground(true);
    mainWindow->setPalette(pal);

    //Begin with multi-threading
    //pthread_t worker_thread;
    //if (pthread_create(&worker_thread, NULL, worker, (void*)1)){
        //qDebug() << "Error: Unable to start worker thread";
       // exit(1);
    //}

    qDebug() << "Beginning main event loop:";

    int exitStatus = a.exec();

    qDebug() << "Exiting main loop:";

    //pthread_exit(NULL); // <- Clean up any loose threads

    return exitStatus;
}
