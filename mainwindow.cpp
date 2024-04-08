#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timerWidget(nullptr)
{
    ui->setupUi(this);
    highlighted = 1;
    currScreen = "Menu";
    menuStrings.push_back("Menu Options:"); // placeholder option to fill menuStrings[0] so that highlighted is the index of the correct option in menuStrings
    menuStrings.push_back("New Session");
    menuStrings.push_back("Session Log");
    menuStrings.push_back("Time and Date");
    connect(ui->upArrowButton, SIGNAL(pressed()), this, SLOT(scroll()));
    connect(ui->downArrowButton, SIGNAL(pressed()), this, SLOT(scroll()));
    connect(ui->menuButton, SIGNAL(released()), this, SLOT(on_menuButton_released()));
    connect(ui->powerButton, SIGNAL(released()), this, SLOT(on_powerButton_released()));
    connect(ui->stopButton, SIGNAL(released()), this, SLOT(on_stopButton_released()));
    connect(ui->playButton, SIGNAL(released()), this, SLOT(on_playButton_released()));
    connect(ui->pauseButton, SIGNAL(released()), this, SLOT(on_pauseButton_released()));

    // for updating the time (still static)
    ui->currTime->setText(QTime::currentTime().toString("h:mm:ss AP")); // seconds will be removed once dyanmic updating is implemented.
    ui->currDate->setText(QDate::currentDate().toString("MMMM d yyyy"));
    // to do Sahil: make the time label update in real time.
    ui->sessionLayout->hide();
    ui->timeAndDateLayout->hide();
    
    foreach(QAbstractButton *button, ui->menuOptions->buttons()){
        ui->menuOptions->setId(button, button->objectName().right(1).toInt());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::scroll() {
    QObject *senderButton = sender();
    ui->menuOptions->button(highlighted)->setStyleSheet("QPushButton{background-color: rgb(153, 193, 241); padding: 0px 25px 0px 25px; border: none; text-align: left; }\n");
    if (senderButton->objectName().left(2) == "up") {
        highlighted -= 1;
        if (highlighted < 1) {
            highlighted = 3;
        }
    } else {
        highlighted += 1;
        if (highlighted > 3) {
            highlighted = 1;
        }
    }
    ui->menuOptions->button(highlighted)->setStyleSheet("QPushButton{background-color: rgb(154, 153, 150); padding: 0px 25px 0px 25px; border: none; text-align: left; }\n");
}

void MainWindow::on_powerButton_released(){}
void MainWindow::on_menuButton_released(){
    ui->sessionLayout->hide();
    ui->timeAndDateLayout->hide();
    ui->menuLayout->show();
}

void MainWindow::on_stopButton_released(){

    MainWindow::stopTimer();
}

void MainWindow::on_playButton_released()
{
    ui->menuLayout->hide(); // hides current menu screen
    if (highlighted==1){ // timer will only be started when the user is on New Session
        MainWindow:createTimer();
        ui->sessionLayout->show();
    } else if (highlighted==2){

    } else {
        ui->timeAndDateLayout->show();
    }
    // sahil to do:  add conditions here which check for which option has been highlighted on the screen. The createTimer() should only be triggered when the 'New Session' option is selected.

}
void MainWindow::on_pauseButton_released(){

    MainWindow::pauseTimer();
}

static int counter = 60; // Initialize counter to 1 minute - page 5 para 3 line 2

void MainWindow::createTimer(){
    // Check if timerWidget already exists, delete it if it does
    if (timerWidget) {
        delete timerWidget;
    }
    // Create a new QWidget for the timer
    timerWidget = new QWidget(this);
    timerWidget->setObjectName("timerWidget");


    QLabel *timerLabel = new QLabel(timerWidget);
    timerLabel->setText("01:00"); // Initial time on countdown
    timerLabel->setAlignment(Qt::AlignHCenter);
    timerLabel->setAlignment(Qt::AlignTop);
    timerLabel->setStyleSheet("QLabel{background-color: rgb(153, 193, 241); font-size: 18px; padding: 0px 5px 70px 5px;}");
    counter=60;

    // Intrnal QTimer to keep track of seconds and decrement based on the interval time I defined below
    QTimer *timer = new QTimer(timerWidget);
    timer->setInterval(1000); // Update every second
    ui->blueLight->setStyleSheet("QPushButton{background-color: rgb(28, 113, 216);}");

    // Connect the timer timeout signal to update the countdown label
    bool connected = connect(timer, &QTimer::timeout, [=]() {
        counter--;
        int minutes = counter / 60;
        int seconds = counter % 60;

        // to print on console for internal checking
        //qDebug() << "Time left: " << minutes << " minutes and " << seconds << " seconds";

        // Updating the timer label
        QString timeString = QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')); // basic formatting
        timerLabel->setText(timeString);

        if (counter == 0) {
            delete timerWidget;
            timerWidget = nullptr;
            ui->blueLight->setStyleSheet("QPushButton{background-color: rgb(153, 193, 241);}");
            //timer->stop(); // Stop the timer
        }
    });

    //ui->timerLayout->addWidget(timerWidget);
    timerWidget->setParent(ui->sessionLayout);
    timerWidget->setGeometry(190, 10, 150, 50);
    timerWidget->setStyleSheet("QWidget{background-color: rgb(153, 193, 241);}");
    timerWidget->show();

    timer->start();
}

void MainWindow::startTreatment() {
    ui->greenLight->setStyleSheet("QPushButton{background-color: rgb(51, 209, 122);}");

    // code for starting treatment here

    ui->greenLight->setStyleSheet("QPushButton{background-color: rgb(143, 240, 164);}");
}

void MainWindow::contactLost() {
    QMessageBox restablishContact;
    ui->redLight->setStyleSheet("QPushButton{background-color: rgb(237, 51, 59);}");
    restablishContact.setText("Alert: Contact has been lost");
    QAbstractButton *responseButton = restablishContact.addButton(tr("Restablish contact"), QMessageBox::AcceptRole);

    int count = 5;
    QTimer countdown;
    bool buttonClicked = true;
    QObject::connect(&countdown, &QTimer::timeout, [&restablishContact, &count, &countdown, &buttonClicked]()->void{
       if (count-- == 0) {
           countdown.stop();
           buttonClicked = false; // the button has not been pressed yet
           restablishContact.close();
       }
    });
    countdown.start(1000);
    restablishContact.exec();
    if (buttonClicked) {
        qInfo("Contact has been restablished");
        ui->redLight->setStyleSheet("QPushButton{background-color: rgb(246, 97, 81);}");
    } else {
        qInfo("Contact has been lost");
    }
}

void MainWindow::stopTimer() {
    if (timerWidget) {
        delete timerWidget;
        timerWidget = nullptr;
    }
}

void MainWindow::pauseTimer() {
    timer->stop();

}



