#include "homewindow.h"
#include "ui_homewindow.h"

#include <qdebug.h>
#include <QMessageBox>
#include <QDate>

#include <opencv2/opencv.hpp>


HomeWindow::HomeWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
    showNoVideoFeed();
    loadYoloModel();
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
        QMessageBox::warning(this, "Camera Error", "Failed to open camera!");
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

            // ---- YOLOv4 Object Detection ----
            cv::Mat blob;
            cv::dnn::blobFromImage(frame, blob, 1/255.0, cv::Size(416, 416), cv::Scalar(), true, false);
            m_net.setInput(blob);

            std::vector<cv::Mat> outputs;
            m_net.forward(outputs, m_net.getUnconnectedOutLayersNames());

            bool detected = false;
            int classId = -1;
            float confidence = 0;

            for (const auto& output : outputs) {
                for (int i = 0; i < output.rows; ++i) {
                    const float* data = output.ptr<float>(i);
                    float score = data[4];
                    if (score > 0.5f) {
                        cv::Mat scores = output.row(i).colRange(5, output.cols);
                        cv::Point classIdPoint;
                        double maxClassScore;
                        cv::minMaxLoc(scores, 0, &maxClassScore, 0, &classIdPoint);
                        if (maxClassScore > 0.6f) {
                            detected = true;
                            classId = classIdPoint.x;
                            confidence = static_cast<float>(maxClassScore);
                            break;
                        }
                    }
                }
                if (detected) break;
            }

            if (detected) {
                if (classId == 0) {
                    m_defectedCount++;
                    ui->label_5->setText("Fail");
                } else {
                    m_undefectedCount++;
                    ui->label_5->setText("Pass");
                }
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

//            std::this_thread::sleep_for(std::chrono::milliseconds(100));  // ~33 FPS
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


    // write json data
    saveSeesionData();
    m_defectedCount = 0;
    m_undefectedCount = 0;

}


void HomeWindow::showNoVideoFeed()
{
    qDebug() << "loading image";
    cv::Mat img = cv::imread("/home/dhruvin/qt_projects/Internship_Project/no-camera-feed.jpg");
    QImage qimage(img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
    ui->label_6->setPixmap(QPixmap::fromImage(qimage));
    ui->label_6->setScaledContents(true);
}

void HomeWindow::saveSeesionData()
{
    // JSON file path (update if needed)
    const std::string filePath = "/home/dhruvin/qt_projects/IQCS/reportData.json";

    JsonHandler handler;
    json data = handler.loadData(filePath);

    // Get today's date as key
    QString qdate = QDate::currentDate().toString("dd-MM-yyyy");
    std::string dateKey = qdate.toStdString();

    // Determine next shift number
    int nextShift = 1;
    if (data.contains(dateKey)) {
        const auto& shifts = data[dateKey];
        if (!shifts.empty()) {
            std::string lastShiftStr = shifts.back().begin().key();
            nextShift = std::stoi(lastShiftStr) + 1;
        }
    }

    // Create new entry for the shift
    json shiftEntry = {
        { std::to_string(nextShift), {
            { "Defected", m_defectedCount },
            { "Undefected", m_undefectedCount }
        }}
    };

    // Append to today’s entry
    data[dateKey].push_back(shiftEntry);

    // Save back to file
    if (handler.saveData(filePath, data)) {
        qDebug() << "JSON updated successfully.";
    } else {
        qDebug() << "Failed to update JSON.";
    }

}

void HomeWindow::on_pushButton_clicked()
{
    startVideoFeed();
}

void HomeWindow::on_pushButton_2_clicked()
{
    stopVideoFeed();
}

void HomeWindow::loadYoloModel()
{
    const std::string modelConfig = "model/yolov4.cfg";
    const std::string modelWeights = "model/yolov4_last.weights";
    const std::string classFile = "model/model.names";

    // Load class names
    std::ifstream ifs(classFile);
    std::string line;
    while (std::getline(ifs, line)) {
        m_classNames.push_back(line);
    }

    // Load the network
    m_net = cv::dnn::readNetFromDarknet(modelConfig, modelWeights);
    qDebug() << "Model loadded successfully";
    m_net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    m_net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
}

void HomeWindow::processDetections(const std::vector<cv::Mat>& outputs, const cv::Mat& frame)
{
    float confThreshold = 0.5;
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    for (const auto& output : outputs) {
        for (int i = 0; i < output.rows; ++i) {
            const auto* data = output.ptr<float>(i);
            float score = data[4];
            if (score > confThreshold) {
                cv::Mat scores = output.row(i).colRange(5, output.cols);
                cv::Point classIdPoint;
                double confidence;

                cv::minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
                if (confidence > confThreshold) {
                    int centerX = static_cast<int>(data[0] * frame.cols);
                    int centerY = static_cast<int>(data[1] * frame.rows);
                    int width = static_cast<int>(data[2] * frame.cols);
                    int height = static_cast<int>(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    classIds.push_back(classIdPoint.x);
                    confidences.push_back(static_cast<float>(confidence));
                    boxes.emplace_back(left, top, width, height);
                }
            }
        }
    }

    // Perform NMS
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, 0.4, indices);

    for (int idx : indices) {
        const auto& box = boxes[idx];
        const auto& label = m_classNames[classIds[idx]];

        // Only interested in custom classes: "defected" / "undefected"
        if (label == "defected")
            ++m_defectedCount;
        else if (label == "undefected")
            ++m_undefectedCount;

        cv::rectangle(frame, box, cv::Scalar(0, 255, 0), 2);
        cv::putText(frame, label, cv::Point(box.x, box.y - 10),
                    cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 1);
    }
}
