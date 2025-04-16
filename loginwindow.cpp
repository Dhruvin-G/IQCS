#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &LoginWindow::on_loginButtonClicked);
}
void LoginWindow::on_loginButtonClicked(){
    emit loginButtonSignalEmit();
}
LoginWindow::~LoginWindow()
{
    delete ui;
}
