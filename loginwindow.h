#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();
signals:
    void loginButtonSignalEmit(QString id, QString password);
private slots:
    void on_loginButtonClicked();
private:
    Ui::LoginWindow *ui;
};

#endif // LOGINWINDOW_H
