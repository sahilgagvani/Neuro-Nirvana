#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QDialog>
#include <QHBoxLayout>
#include <QTime>
#include <QLabel>
#include <QDate>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
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
    void createTimer();
    void startTreatment();
    void stopTimer();
    void pauseTimer();

private:
    Ui::MainWindow *ui;
    std::list<QString> menuStrings; // list of the menu options displayed on the screen
    int highlighted; // holds the index of the currently selected menu options
    QString currScreen; // holds the current screen - menu, new session, etc.
    QWidget *timerWidget;
    QLabel *timerlabel;
    QTimer *timer;
};
#endif // MAINWINDOW_H
