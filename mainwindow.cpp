#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QRegularExpression>
#include <QMessageBox>
#include<QDebug>
#include "jsonhandler.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loginwindow = new LoginWindow(this);
    ui->horizontalLayout->addWidget(loginwindow);
    loginwindow->setStyleSheet("#verticalWidget{Border:2px solid rgb(23, 21, 59)}");

//    this->setStyleSheet("background-color: purple");
    connect(loginwindow, &LoginWindow::loginButtonSignalEmit, this, &MainWindow::loginButtonClicked);
}

void MainWindow::loginButtonClicked(QString id, QString password) {
    // ID Validation
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "ID field cannot be empty.");
        return;
    }
    // Password Validation
    if (password.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Password field cannot be empty.");
        return;
    }

    // Load user JSON
    JsonHandler handler;
    const std::string filePath = "/home/dhruvin/qt_projects/IQCS/userData.json"; // 🔁 Update this path
    json userData = handler.loadData(filePath);
    qDebug() << QString::fromStdString(userData.dump());


    bool matchFound = false;

    if (userData.contains("user")) {
        for (const auto& user : userData["user"]) {
            std::string storedId = user.value("id", "");
            std::string storedPass = user.value("password", "");

            if (storedId == id.toStdString() && storedPass == password.toStdString()) {
                matchFound = true;
                break;
            }
        }
    }

    if (!matchFound) {
        QMessageBox::warning(this, "Login Failed", "Invalid ID or Password.");
        return;
    }

    // ✅ Proceed to load the main window (same as your current implementation)
    verticalLayout = new QVBoxLayout(this);
    header = new Header(this);
    home = new HomeWindow(this);
    this->setStyleSheet("background-color:white");
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


//void MainWindow::loginButtonClicked(QString id, QString password){

//        // ID Validation
//        if (id.isEmpty()) {
//            QMessageBox::warning(this, "Input Error", "ID field cannot be empty.");
//            return;
//        }
//        // Password Validation
//        if (password.isEmpty()) {
//            QMessageBox::warning(this, "Input Error", "Password field cannot be empty.");
//            return;
//        }
//        if (id != "abc123") {
//            QMessageBox::warning(this, "Input Error", "ID must contain only letters and numbers.");
//            return;
//        }
//        if (password != "abc@2023") {
//            QMessageBox::warning(this, "Input Error", "Password must be at least 9 characters long and include one lowercase, one uppercase, one number, and one special character (!@&%#$^*).");
//            return;
//        }


//        else{
//            verticalLayout = new QVBoxLayout(this);
//            header = new Header(this);
//            home = new HomeWindow(this);
//            this->setStyleSheet("background-color:white");
//            report = new Report(this);
//            report->hide();
//            header->setAttribute(Qt::WA_StyledBackground, true);
//            header->enbleHomeButton(false);

//            verticalLayout->addWidget(header);
//            verticalLayout->addWidget(home);
//            ui->horizontalLayout->addLayout(verticalLayout);

//            connect(header, &Header::logoutButtonClickedSignal, this, &MainWindow::logoutButtonClicked);
//            connect(header, &Header::closeButtonClickedSignal, this, &MainWindow::closeButtonClicked);
//            connect(header, &Header::reportButtonClickedSignal, this, &MainWindow::reportButtonClicked);
//            connect(header, &Header::homeButtonClickedSignal, this, &MainWindow::homeButtonClicked);
//        }
//    loginwindow->deleteLater();
//}


void MainWindow::logoutButtonClicked(){
//    if (header) header->deleteLater();
    header->hide();
    report->hide();
    home->hide();
    loginwindow = new LoginWindow(this);
    ui->horizontalLayout->addWidget(loginwindow);
    loginwindow->setStyleSheet("#verticalWidget{Border:2px solid rgb(23, 21, 59)}");
    loginwindow->setStyleSheet("background-color:purple");

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
    report->fetchSessionData();
    home->hide();
    verticalLayout->addWidget(report);
    header->enbleHomeButton(true);
    header->enbleReportButton(false);

}
MainWindow::~MainWindow()
{
    delete ui;
    header->deleteLater();
    report->deleteLater();
    home->deleteLater();
    qDebug()<<"main destructor";
}
