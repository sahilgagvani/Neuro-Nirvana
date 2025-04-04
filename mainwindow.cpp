#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_graphwindow.h"
#include "graphwindow.h"
#include "qdebug.h"
#include <iostream>
#include <fstream>
#include <QMessageBox>
#include <QGridLayout>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , timerWidget(nullptr)
    , disconnectTimerWidget(nullptr), gridLayout(new QGridLayout())
{
    ui->setupUi(this);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->widget()->setLayout(gridLayout);
    graphWin = new GraphWindow();
    highlighted = 1;
    remainingTime = 0;
    currScreen = "Menu";
    headset = new CentralProcessor();
    menuStrings.push_back("Menu Options:"); // placeholder option to fill menuStrings[0] so that highlighted is the index of the correct option in menuStrings
    menuStrings.push_back("New Session");
    menuStrings.push_back("Session Log");
    menuStrings.push_back("Time and Date");
    connect(ui->upArrowButton, SIGNAL(pressed()), this, SLOT(scroll()));
    connect(ui->downArrowButton, SIGNAL(pressed()), this, SLOT(scroll()));
    connect(graphWin->ui->electrodeDropdown, SIGNAL(currentIndexChanged(int)), this, SLOT(getElectrodeGraph(int)));
    connect(this, SIGNAL(getElectrodeInfo(int)), headset, SLOT(singleElectrodeGraph(int)));
    connect(headset, SIGNAL(graphUpdate(int, int)), this, SLOT(drawGraph(int, int)));
    connect(headset, SIGNAL(sendSaveData(int,int)), this, SLOT(saveBaselines(int,int)));

    MainWindow::disableButtons(true);// disables buttons until power button is pressed

    QTime currTime = QTime();
    QDate currDate = QDate();
    date = new QDateTimeEdit(currDate.currentDate(),ui->currDate);
    time = new QDateTimeEdit(currTime.currentTime(),ui->currTime);
    date->setDisplayFormat("yyyy-MM-dd");
    time ->setDisplayFormat("HH:mm:ss");
    // for updating the time (still static)
    //ui->currTime->setText(QTime::currentTime().toString("h:mm:ss AP")); // seconds will be removed once dyanmic updating is implemented.
    //ui->currDate->setText(QDate::currentDate().toString("MMMM d yyyy"));
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

    batteryLevel = 4; //initializing battery bar
}

MainWindow::~MainWindow()
{
    //remove("savedLogs.txt");
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
    if (batteryLevel <= 0) {
        batteryLevel = 4;
        updateBatteryLevel();
    }

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
    } else if (ui->timeAndDateLayout->isVisible()){
        ui->timeAndDateLayout->hide();
    }

    if (ui->menuLayout->isVisible()){ // if we are currently on the menu screen, go to the session screen
        ui->sessionLayout->show();
        ui->menuLayout->hide();
    } else {                          // otherwise go to the menu screen
        ui->menuLayout->show();
    }
}

void MainWindow::on_stopButton_released(){

    MainWindow::stopTimer();
}

void MainWindow::on_playButton_released()
{
    if (disconnectTimerRunning) { // reestablish contact
        ui->disconnectLayout->hide();
        ui->sessionLayout->show();
        disconnectTimerRunning = false;
        MainWindow::startTreatment(); // resumes treatment session
    } else {
        ui->menuLayout->hide(); // hides current menu screen
        if (highlighted==1){ // timer will only be started when the user is on New Session
            ui->disconnectButton->setDisabled(false);
            MainWindow::startTreatment();
            ui->sessionLayout->show();
        } else if (highlighted==2){
            ui->logLayout->show();
            MainWindow::displaySessionLogs();
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


void MainWindow::drawGraph(int amp, int freq){
    // illuminate green light when treatment is being delivered
    ui->greenLight->setStyleSheet("QPushButton{background-color: rgb(51, 209, 122);}");
    graphWin->show();

    double period = (2*M_PI) / freq;

    // Generate points to plot
    QVector<double> x(400), y(400);

    for (int i=0; i<400; ++i){
      x[i] = i/10.0 - freq;
      y[i] = amp * qSin(period * x[i]);
    }

    graphWin->ui->wavePlot->addGraph();
    graphWin->ui->wavePlot->graph(0)->setData(x, y);

    graphWin->ui->wavePlot->xAxis->setLabel("x");
    graphWin->ui->wavePlot->yAxis->setLabel("y");

    graphWin->ui->wavePlot->xAxis->setRange(-freq, freq);
    graphWin->ui->wavePlot->yAxis->setRange(-amp, amp);

    graphWin->ui->wavePlot->replot();
}


static int counter = 29; // Initialize counter to 1 minute - page 5 para 3 line 2
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
    ui->progressBar->setValue(0);

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
        ui->progressBar->setValue(100 - seconds /29.0 * 100);

        if (counter == 0) {
            batteryLevel--;
            if (batteryLevel <= 0) {
                MainWindow::endTreatment();
                MainWindow::on_powerButton_released();

            } else {
                counter = 60;
                createTimer();
            }

            saveData(initialBaseline,finalBaseline);
            delete timerWidget;
            timerWidget = nullptr;
            graphWin->hide();
            ui->greenLight->setStyleSheet("QPushButton{background-color: rgb(143, 240, 164);}");
            ui->blueLight->setStyleSheet("QPushButton{background-color: rgb(153, 193, 241);}");
            counter=60;
            //timer->stop();
            updateBatteryLevel();
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
    if(batteryLevel>0){
        ui->greenLight->setStyleSheet("QPushButton{background-color: rgb(143, 240, 164);}");
        createTimer();
        headset->applyFullTreatment();
    } else {
        QMessageBox batteryEmpty;
        batteryEmpty.setText("Error: Battery is empty. Cannot start treatment.");
        batteryEmpty.exec();
    }
}

void MainWindow::endTreatment() {
    QMessageBox batteryDead;
    batteryLevel = 0;
    updateBatteryLevel();
    batteryDead.setText("Alert: Battery is dead. Powering off");
    batteryDead.exec();
    qInfo("Powering off");
    ui->sessionLayout->hide();
    MainWindow::on_powerButton_released();
}

void MainWindow::getElectrodeGraph(int index) {
    emit getElectrodeInfo(index - 1);
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

    // turn blue/green light off and red light on
    ui->greenLight->setStyleSheet("QPushButton{background-color: rgb(143, 240, 164);}");
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
}

void MainWindow::saveData(int initialBaseline, int finalBaseline){
    fstream dataFile;
    dataFile.open("savedLogs.txt",fstream::app);

    if(!dataFile){
        dataFile.open("savedLogs.txt",fstream::out);
        qDebug()<<"Creating file";
    }
    QDate currDate = date->date();
    QTime currTime = time->time();
    QString dateString = currDate.currentDate().toString("MMMM d yyyy");
    QString timeString = currTime.currentTime().toString("hh:mm:ss");
    dataFile << initialBaseline << "                             " << finalBaseline <<"                    " << dateString.toStdString() << " " << timeString.toStdString()<<"\n";
    qDebug()<< "wrote to file";

    dataFile.close();
}

void MainWindow::saveBaselines(int iBaseline, int fBaseline){
    initialBaseline = iBaseline;
    finalBaseline = fBaseline;
}

void MainWindow::displaySessionLogs(){
    fstream dataFile;
    dataFile.open("savedLogs.txt",fstream::in);

    if(dataFile){
        deletePreSessions();
        string line;
        //int row = 0;
        //int column = 0;
        while(getline(dataFile,line)){
            char arr[line.length() + 1];
            strcpy(arr,line.c_str());
            QLabel *info = new QLabel(arr);
            ui->scrollArea->widget()->layout()->addWidget(info);
        }
    }

    dataFile.close();
}

void MainWindow::deletePreSessions(){
    QLayoutItem* widget;
    while((widget = ui->scrollArea->widget()->layout()->takeAt(0)) != 0){
        widget->widget()->setParent(NULL);
        delete widget;
    }
}


void MainWindow::updateBatteryLevel(){
    QString batteryImagePath;

    switch(batteryLevel){
    case 4:
        batteryImagePath = ":/battery_4.jpeg";
        break;
    case 3:
        batteryImagePath = ":/battery_3.jpeg";
        break;
    case 2:
        batteryImagePath = ":/battery_2.jpeg";
        break;
    case 1:
        batteryImagePath = ":/battery_1.jpeg";
        break;
    default:
        batteryImagePath = ":/battery_0.jpeg";
        break;
    }

    QPixmap batteryPixMap(batteryImagePath);
    ui->batteryBarImage->setPixmap(batteryPixMap);
}

