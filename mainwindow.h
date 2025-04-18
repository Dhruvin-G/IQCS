#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"loginwindow.h"
#include"header.h"
#include"report.h"
#include"homewindow.h"
#include<QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void loginButtonClicked(QString id, QString password);
    void logoutButtonClicked();
    void homeButtonClicked();
    void closeButtonClicked();
    void reportButtonClicked();
private:
    Ui::MainWindow *ui;
    QVBoxLayout *verticalLayout;
    LoginWindow *loginwindow;
    Header *header;
    Report *report;
    HomeWindow *home;

};
#endif // MAINWINDOW_H
