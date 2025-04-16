#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <thread>

#include <opencv2/opencv.hpp>

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();
    void startVideoFeed();
    void stopVideoFeed();
    void showNoVideoFeed();

    std::thread m_cameraCaptureThread;
    cv::VideoCapture m_cap;
    cv::Mat m_frame;
    QImage m_qimage;
    std::atomic<bool> m_cameraRunning = false;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::HomeWindow *ui;

    int m_defectedCount = 0;
    int m_undefectedCount = 0;
};

#endif // HOMEWINDOW_H
