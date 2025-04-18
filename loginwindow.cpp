#include "loginwindow.h"
#include "ui_loginwindow.h"

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->lineEdit->setStyleSheet("color: rgb(27, 26, 85); font-size: 26px; font-weight: 580; min-height:44");
    ui->lineEdit_2->setStyleSheet("color: rgb(27, 26, 85); font-size: 26px; font-weight: 580; min-height:44");
    connect(ui->pushButton, &QPushButton::clicked, this, &LoginWindow::on_loginButtonClicked);
}
void LoginWindow::on_loginButtonClicked(){
    emit loginButtonSignalEmit(ui->lineEdit->text(), ui->lineEdit_2->text());
}
LoginWindow::~LoginWindow()
{
    delete ui;
}
