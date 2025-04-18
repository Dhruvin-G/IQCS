#include "homewindow.h"
#include "ui_homewindow.h"

#include <opencv2/opencv.hpp>

#include <gvision.h>

HomeWindow::HomeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_StyledBackground, true);
    this->setStyleSheet("background-color: #FFFFFF");
    cv::VideoCapture cap(0);

//    showVideoFeed();
}



HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::showVideoFeed()
{
    auto ctl = CTlFactory::getInstance(TransportLayerType::USB);
    ctl->enumerateDevices();
    Camera cam(ctl->createDevice(0));
    cam.startGrabbing();
    cv::Mat img = cv::imread("/home/dhruvin/Pictures/Mountain.jpg");

//    cam.getImage(img);

    cam.stopGrabbing();


    QImage qimage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);

    ui->label_6->setPixmap(QPixmap::fromImage(qimage));
    ui->label_6->setScaledContents(true);
}
