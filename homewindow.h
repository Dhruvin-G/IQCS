#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <thread>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "jsonhandler.h"


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
    void saveSeesionData();

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

    cv::dnn::Net m_net;
    std::vector<std::string> m_classNames;


    void loadYoloModel();
    void processDetections(const std::vector<cv::Mat>& outputs, const cv::Mat& frame);
};

#endif // HOMEWINDOW_H
