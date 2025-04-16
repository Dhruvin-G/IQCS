#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loginwindow = new LoginWindow(this);
    ui->horizontalLayout->addWidget(loginwindow);
    loginwindow->setStyleSheet("#verticalWidget{Border:2px solid rgb(23, 21, 59)}");

    connect(loginwindow, &LoginWindow::loginButtonSignalEmit, this, &MainWindow::loginButtonClicked);
}
void MainWindow::loginButtonClicked(){
    verticalLayout = new QVBoxLayout(this);
    header = new Header(this);
    home = new HomeWindow(this);
    report = new Report(this);
    report->hide();
    header->setAttribute(Qt::WA_StyledBackground, true);
    header->enbleHomeButton(false);

    verticalLayout->addWidget(header);
    verticalLayout->addWidget(home);
    ui->horizontalLayout->addLayout(verticalLayout);

    connect(header, &Header::logoutButtonClickedSignal, this, &MainWindow::logoutButtonClicked);
    connect(header, &Header::closeButtonClickedSignal, this, &MainWindow::closeButtonClicked);
    connect(header, &Header::reportButtonClickedSignal, this, &MainWindow::reportButtonClicked);
    connect(header, &Header::homeButtonClickedSignal, this, &MainWindow::homeButtonClicked);

    loginwindow->deleteLater();
}
void MainWindow::logoutButtonClicked(){
//    if (header) header->deleteLater();
    header->hide();
    report->hide();
    home->hide();
    if (report) report->deleteLater();
    loginwindow = new LoginWindow(this);
    ui->horizontalLayout->addWidget(loginwindow);
    loginwindow->setStyleSheet("#verticalWidget{Border:2px solid rgb(23, 21, 59)}");

    connect(loginwindow, &LoginWindow::loginButtonSignalEmit, this, &MainWindow::loginButtonClicked);

}
void MainWindow::homeButtonClicked(){
    home->show();
    report->hide();
    verticalLayout->addWidget(home);
    header->enbleHomeButton(false);
    header->enbleReportButton(true);

}
void MainWindow::closeButtonClicked(){
    this->close();
}

void MainWindow::reportButtonClicked(){
    report->show();
    home->hide();
    verticalLayout->addWidget(report);
    header->enbleHomeButton(true);
    header->enbleReportButton(false);

}
MainWindow::~MainWindow()
{
    header->deleteLater();
    report->deleteLater();
    home->deleteLater();
    delete ui;
    delete loginwindow;
}
