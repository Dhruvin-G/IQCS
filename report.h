#ifndef REPORT_H
#define REPORT_H

#include <QWidget>

namespace Ui {
class Report;
}

class Report : public QWidget
{
    Q_OBJECT

public:
    explicit Report(QWidget *parent = nullptr);
    ~Report();
private slots:
    void on_exportPDFButtonClicked();
private:
    Ui::Report *ui;
    void setPieChart();
};

#endif // REPORT_H
