#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QPixmap pixmap("/home/student/3004final/hamburgerMenu.png");
    QIcon ButtonIcon(pixmap);
    ui->menuButton->setIcon(ButtonIcon);
    ui->menuButton->setIconSize(pixmap.rect().size());
}

MainWindow::~MainWindow()
{
    delete ui;
}

