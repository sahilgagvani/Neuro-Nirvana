#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include <iostream>
#include <fstream>
#include <QMessageBox>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timerWidget(nullptr)
    , disconnectTimerWidget(nullptr)
{
    ui->setupUi(this);
    highlighted = 1;
    remainingTime = 0;
    currScreen = "Menu";
    menuStrings.push_back("Menu Options:"); // placeholder option to fill menuStrings[0] so that highlighted is the index of the correct option in menuStrings
    menuStrings.push_back("New Session");
    menuStrings.push_back("Session Log");
    menuStrings.push_back("Time and Date");
    connect(ui->upArrowButton, SIGNAL(pressed()), this, SLOT(scroll()));
    connect(ui->downArrowButton, SIGNAL(pressed()), this, SLOT(scroll()));

    MainWindow::disableButtons(true);// disables buttons until power button is pressed

    // for updating the time (still static)
    ui->currTime->setText(QTime::currentTime().toString("h:mm:ss AP")); // seconds will be removed once dyanmic updating is implemented.
    ui->currDate->setText(QDate::currentDate().toString("MMMM d yyyy"));
    // to do Sahil: make the time label update in real time.

    ui->menuLayout->hide();
    ui->sessionLayout->hide();
    ui->logLayout->hide();
    ui->timeAndDateLayout->hide();
    ui->disconnectLayout->hide();
    
    foreach(QAbstractButton *button, ui->menuOptions->buttons()){
        ui->menuOptions->setId(button, button->objectName().right(1).toInt());
    }
    disconnectTimerRunning = false;
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

void MainWindow::disableButtons(bool disableValue) {
    ui->playButton->setDisabled(disableValue);
    ui->pauseButton->setDisabled(disableValue);
    ui->stopButton->setDisabled(disableValue);
    ui->upArrowButton->setDisabled(disableValue);
    ui->downArrowButton->setDisabled(disableValue);
    ui->menuButton->setDisabled(disableValue);
    if (disableValue) {
        ui->disconnectButton->setDisabled(disableValue);

        // turn off lights
        ui->blueLight->setStyleSheet("QPushButton{background-color: rgb(153, 193, 241);}");
        ui->redLight->setStyleSheet("QPushButton{background-color: rgb(246, 97, 81);}");
        ui->greenLight->setStyleSheet("QPushButton{background-color: rgb(143, 240, 164);}");
    }
}

void MainWindow::on_powerButton_released(){
    if (ui->menuLayout->isVisible()) {
        ui->menuLayout->hide();
        MainWindow::disableButtons(true);
    } else if (ui->sessionLayout->isVisible()){
        ui->sessionLayout->hide();
        MainWindow::disableButtons(true);
    } else if (ui->logLayout->isVisible()) {
        ui->logLayout->hide();
        MainWindow::disableButtons(true);
    } else if (ui->timeAndDateLayout->isVisible()) {
        ui->timeAndDateLayout->hide();
        MainWindow::disableButtons(true);
    } else if (ui->disconnectLayout->isVisible()) {
        ui->disconnectLayout->hide();
        MainWindow::disableButtons(true);
    } else {
        ui->menuLayout->show();
        MainWindow::disableButtons(false);
    }
}

void MainWindow::on_menuButton_released(){
    if (ui->sessionLayout->isVisible()){
        ui->disconnectButton->setDisabled(true);
        ui->sessionLayout->hide();
    } else if (ui->logLayout->isVisible()) {
        ui->logLayout->hide();
    } else {
        ui->timeAndDateLayout->hide();
    }
    ui->menuLayout->show();
}

void MainWindow::on_stopButton_released(){

    MainWindow::stopTimer();
}

void MainWindow::on_playButton_released()
{
<<<<<<< HEAD
    ui->menuLayout->hide(); // hides current menu screen
    if (highlighted==1){ // timer will only be started when the user is on New Session
        MainWindow::startTreatment();
        MainWindow::createTimer();
        ui->sessionLayout->show();
    } else if (highlighted==2){
        MainWindow::displaySessionLogs();
=======
    if (disconnectTimerRunning) { // reestablish contact
        ui->disconnectLayout->hide();
        ui->sessionLayout->show();
        disconnectTimerRunning = false;
        MainWindow::startTreatment(); // resumes treatment session
>>>>>>> 445f962 (Added disconnect button and timer for testing, refined lights)
    } else {
        ui->menuLayout->hide(); // hides current menu screen
        if (highlighted==1){ // timer will only be started when the user is on New Session
            ui->disconnectButton->setDisabled(false);
            MainWindow::startTreatment();
            ui->sessionLayout->show();
        } else if (highlighted==2){
            ui->logLayout->show();
        } else {
            ui->timeAndDateLayout->show();
        }
        // sahil to do:  add conditions here which check for which option has been highlighted on the screen. The createTimer() should only be triggered when the 'New Session' option is selected.
    }
}
void MainWindow::on_pauseButton_released(){

    MainWindow::pauseTimer();
}

void MainWindow::on_disconnectButton_released(){
    MainWindow::contactLost();
}

static int counter = 60; // Initialize counter to 1 minute - page 5 para 3 line 2
static int counter2 = 10;

void MainWindow::createTimer(){

    // Check if timerWidget already exists, delete it if it does
    if (timerWidget) {
        delete timerWidget;
    }
    // Create a new QWidget for the timer
    timerWidget = new QWidget(this);
    timerWidget->setObjectName("timerWidget");

    QLabel *timerLabel = new QLabel(timerWidget);
    timerLabel->setMinimumWidth(250);
    timerLabel->setText(QString("%1:%2").arg(counter / 60, 1, 10, QChar('0')).arg(counter % 60, 2, 10, QChar('0'))); // Initial time on countdown
    timerLabel->setAlignment(Qt::AlignHCenter);
    timerLabel->setAlignment(Qt::AlignTop);
    timerLabel->setStyleSheet("QLabel{background-color: rgb(153, 193, 241); font-size: 18px; padding: 0px 5px 70px 25px;}");

    // Intrnal QTimer to keep track of seconds and decrement based on the interval time I defined below
    timer = new QTimer(timerWidget);
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
        QString timeString = QString("%1:%2").arg(minutes, 1, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')); // basic formatting
        timerLabel->setText(timeString);

        if (counter == 0) {
            delete timerWidget;
            timerWidget = nullptr;
            ui->blueLight->setStyleSheet("QPushButton{background-color: rgb(153, 193, 241);}");
            counter=60;
            //timer->stop(); // Stop the timer
        }
    });

    timerWidget->setParent(ui->sessionLayout);
    timerWidget->setGeometry(190, 10, 250, 50);
    timerWidget->setStyleSheet("QWidget{background-color: red;}");
    timerWidget->show();

    timer->start();
}

void MainWindow::startTreatment() {
    ui->greenLight->setStyleSheet("QPushButton{background-color: rgb(51, 209, 122);}");

    // code for starting treatment here

    ui->greenLight->setStyleSheet("QPushButton{background-color: rgb(143, 240, 164);}");
    createTimer();
    if (counter == 60 ){
        ui->batteryBar->setValue(ui->batteryBar->value() - 33);
    }

    if ((ui->batteryBar->value() <= 1)) {
        QMessageBox batteryDead;
        batteryDead.setText("Alert: Battery is dead. Powering off");
        batteryDead.exec();
        qInfo("Powering off");
        ui->sessionLayout->hide();
        MainWindow::on_powerButton_released();
    }
}

void MainWindow::contactLost() {
    disconnectTimerRunning = true;
    if (ui->menuLayout->isVisible()) {
        ui->menuLayout->hide();
    } else if (ui->sessionLayout->isVisible()){
        ui->sessionLayout->hide();
    } else if (ui->logLayout->isVisible()) {
        MainWindow::disableButtons(true);
    } else if (ui->timeAndDateLayout->isVisible()) {
        ui->timeAndDateLayout->hide();
    }
    ui->disconnectLayout->show();
    // Check if timerWidget already exists, delete it if it does
    if (disconnectTimerWidget) {
        delete disconnectTimerWidget;
    }
    // Create a new QWidget for the timer
    disconnectTimerWidget = new QWidget(this);
    disconnectTimerWidget->setObjectName("disconnectTimerWidget");

    counter2 = 10;

    QLabel *timerLabel = new QLabel(disconnectTimerWidget);
    timerLabel->setText(QString("%1:%2").arg(counter2 / 60, 1, 10, QChar('0')).arg(counter2 % 60, 2, 10, QChar('0'))); // Initial time on countdown
    timerLabel->setAlignment(Qt::AlignHCenter);
    timerLabel->setAlignment(Qt::AlignTop);
    timerLabel->setStyleSheet("QLabel{background-color: rgb(153, 193, 241); font-size: 18px; padding: 0px 5px 70px 5px;}");

    // Intrnal QTimer to keep track of seconds and decrement based on the interval time I defined below
    QTimer* disconnectTimer = new QTimer(disconnectTimerWidget);
    disconnectTimer->setInterval(1000); // Update every second

    // turn blue light off and red light on
    ui->blueLight->setStyleSheet("QPushButton{background-color: rgb(153, 193, 241);}");
    ui->redLight->setStyleSheet("QPushButton{background-color: rgb(237, 51, 59);}");

    // Connect the timer timeout signal to update the countdown label
    bool connected = connect(disconnectTimer, &QTimer::timeout, [=]() {
        counter2--;
        int minutes2 = counter2 / 60;
        int seconds2 = counter2 % 60;

        // Updating the timer label
        QString timeString = QString("%1:%2").arg(minutes2, 1, 10, QChar('0')).arg(seconds2, 2, 10, QChar('0')); // basic formatting
        timerLabel->setText(timeString);

        if (counter2 == 0 && disconnectTimerRunning == true) {
            disconnectTimerRunning = false;
            delete timerWidget;
            timerWidget = nullptr;
            MainWindow::on_powerButton_released();
        }
    });
    disconnectTimerWidget->setParent(ui->disconnectLayout);
    disconnectTimerWidget->setGeometry(80, 104, 150, 50);
    disconnectTimerWidget->setStyleSheet("QWidget{background-color: rgb(153, 193, 241);}");
    disconnectTimerWidget->show();

    disconnectTimer->start();

}

void MainWindow::stopTimer() {
    if (timerWidget) {
        counter = 60;
        delete timerWidget;
        timerWidget = nullptr;
        ui->blueLight->setStyleSheet("QPushButton{background-color: rgb(153, 193, 241);}");
    }
}

void MainWindow::pauseTimer() {
    //remainingTime = counter % 60;
    ui->blueLight->setStyleSheet("QPushButton{background-color: rgb(153, 193, 241);}");
    timer->stop();
<<<<<<< HEAD
}

void MainWindow::displaySessionLogs(){
    fstream dataFile;
    dataFile.open("savedLogs.txt",fstream::in);

    if(dataFile){

    }

    dataFile.close();

=======
>>>>>>> 445f962 (Added disconnect button and timer for testing, refined lights)
}


