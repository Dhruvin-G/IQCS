#include "homewindow.h"
#include "ui_homewindow.h"

#include <opencv2/opencv.hpp>


HomeWindow::HomeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    cv::VideoCapture cap(0);

    showVideoFeed();
}



HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::showVideoFeed()
{
    cv::Mat img = cv::imread("/home/dhruvin/Pictures/Mountain.jpg");


    QImage qimage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);

    ui->label_6->setPixmap(QPixmap::fromImage(qimage));
    ui->label_6->setScaledContents(true);
}
