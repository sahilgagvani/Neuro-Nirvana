#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDialog>
#include <QHBoxLayout>
#include <QDateTime>
#include <QLabel>
#include <QDate>
#include <QTime>
#include <QDateTimeEdit>
#include <cmath>
#include "CentralProcessor.h"
#include "qcustomplot.h"
#include "graphwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void scroll();
    void on_powerButton_released();
    void on_menuButton_released();
    void on_stopButton_released();
    void on_playButton_released();
    void on_pauseButton_released();
    void on_disconnectButton_released();
    void createTimer();
    void startTreatment();
    void stopTimer();
    void pauseTimer();
    void contactLost();
    void displaySessionLogs();
    void drawGraph(int, int);
    void getElectrodeGraph(int);
    void saveData(int,int);
    void updateBatteryLevel();
    void saveBaselines(int,int);
    void endTreatment();

private:
    Ui::MainWindow *ui;
    std::list<QString> menuStrings; // list of the menu options displayed on the screen
    int highlighted; // holds the index of the currently selected menu options
    QString currScreen; // holds the current screen - menu, new session, etc.
    QWidget *timerWidget;
    QWidget *disconnectTimerWidget;
    QLabel *timerlabel;
    QTimer *timer;
    QDateTimeEdit* date;
    QDateTimeEdit* time;
    GraphWindow* graphWin;
    CentralProcessor* headset;
    QGridLayout *gridLayout;
    bool disconnectTimerRunning;
    int remainingTime;
    void disableButtons(bool);
    void deletePreSessions();
    int batteryLevel;
    int initialBaseline;
    int finalBaseline;

signals:
    void getElectrodeInfo(int);
};
#endif // MAINWINDOW_H
