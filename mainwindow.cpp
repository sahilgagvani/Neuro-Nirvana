#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
    connect(ui->upArrowButton, SIGNAL(released()), this, SLOT(on_upArrowButton_released()));
    connect(ui->downArrowButton, SIGNAL(released()), this, SLOT(on_downArrowButton_released()));
    connect(ui->stopButton, SIGNAL(released()), this, SLOT(on_stopButton_released()));
    connect(ui->playButton, SIGNAL(released()), this, SLOT(on_playButton_released()));
    connect(ui->pauseButton, SIGNAL(released()), this, SLOT(on_pauseButton_released()));
    
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

void MainWindow::on_powerbutton_released(){}
void MainWindow::on_menueButton_released(){}
void MainWindow::on_upArrowButton_released(){}
void MainWindow::on_downArrowButton_released(){}
void MainWindow::on_stopButton_released(){}
void MainWindow::on_playButton_released(){}
void MainWindow::on_pauseButton_released(){}
