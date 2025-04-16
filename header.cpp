#include "header.h"
#include "ui_header.h"

Header::Header(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Header)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &Header::on_logoutButtonClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &Header::on_homeButtonClicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &Header::on_closeButtonClicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &Header::on_reportButtonClicked);
}

void Header::enbleReportButton(bool _flag)
{
    ui->pushButton_3->setVisible(_flag);
}

void Header::enbleHomeButton(bool _flag)
{
    ui->pushButton_2->setVisible(_flag);
}
void Header::on_logoutButtonClicked(){
    emit logoutButtonClickedSignal();
}
void Header::on_homeButtonClicked(){
    emit homeButtonClickedSignal();
}
void Header::on_closeButtonClicked(){
    emit closeButtonClickedSignal();
}

void Header::on_reportButtonClicked(){
    emit reportButtonClickedSignal();
}
Header::~Header()
{
    delete ui;
}
