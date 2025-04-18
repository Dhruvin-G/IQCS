#include "homewindow.h"
#include "ui_homewindow.h"

#include <qdebug.h>

#include <opencv2/opencv.hpp>


HomeWindow::HomeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    showNoVideoFeed();
}



HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::startVideoFeed()
{
    // Open webcam once and check
    if (!m_cap.open(0)) {
        qDebug() << "Error: Cannot open webcam";
        return;
    }

    qDebug() << "Camera opened";

    m_cameraRunning = true;

    m_cameraCaptureThread = std::thread([this]() {
        while (m_cameraRunning) {
            cv::Mat frame;
            m_cap >> frame;

            if (frame.empty()) {
                qDebug() << "Blank frame grabbed";
                continue;
            }


            // Image Detection
            // Simulate detection
            int mockClassId = rand() % 2;
            float mockConfidence = static_cast<float>(rand()) / RAND_MAX;

            if (mockConfidence > 0.6f) {
                if (mockClassId == 0)
                    m_defectedCount++;
                else
                    m_undefectedCount++;
            }

            // Convert BGR to RGB
            cv::Mat rgbFrame;
            cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);

            // Create a deep copy of the QImage
            QImage qimg(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
            QImage imageCopy = qimg.copy();

            // Push to UI thread
            QMetaObject::invokeMethod(this, [this, imageCopy]() {
                // update video frame in ui
                ui->label_6->setPixmap(QPixmap::fromImage(imageCopy));
                ui->label_6->setScaledContents(true);

                // update count in ui
                ui->label->setText(QString::number(m_defectedCount));
                ui->label_2->setText(QString::number(m_undefectedCount));
//                ui->labelDefected->setText(QString::number(defectedCount));
            }, Qt::QueuedConnection);

            std::this_thread::sleep_for(std::chrono::milliseconds(300));  // ~33 FPS
        }

        qDebug() << "Capture thread exited";
    });
}


void HomeWindow::stopVideoFeed()
{
    if (!m_cameraRunning) {
        return;  // Already stopped
    }

    qDebug() << "Stopping video feed...";
    m_cameraRunning = false;

    if (m_cameraCaptureThread.joinable()) {
        m_cameraCaptureThread.join();
    }

    if (m_cap.isOpened()) {
        m_cap.release();
    }

    // Optionally clear the label
    showNoVideoFeed();
    qDebug() << "Video feed stopped";
}


void HomeWindow::showNoVideoFeed()
{
    cv::Mat img = cv::imread("/home/dhruvin/qt_projects/Internship_Project/no-camera-feed.jpg");
    QImage qimage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    ui->label_6->setPixmap(QPixmap::fromImage(qimage));
    ui->label_6->setScaledContents(true);
}

void HomeWindow::on_pushButton_clicked()
{
    startVideoFeed();
}

void HomeWindow::on_pushButton_2_clicked()
{
    stopVideoFeed();
}
