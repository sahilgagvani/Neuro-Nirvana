#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

private:
    Ui::MainWindow *ui;
    std::list<QString> menuStrings; // list of the menu options displayed on the screen
    int highlighted; // holds the index of the currently selected menu options
    QString currScreen; // holds the current screen - menu, new session, etc.
};
#endif // MAINWINDOW_H
