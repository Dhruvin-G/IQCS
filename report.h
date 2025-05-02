#ifndef REPORT_H
#define REPORT_H
#include <QtCharts>
#include <QDate>
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
    void fetchSessionData();
    void setPieChart();
private slots:
    void on_exportPDFButtonClicked();
    void on_dateChange(QDate date);
private:
    Ui::Report *ui;
    QPieSlice *defectedSlice;
    QPieSlice *undefectedSlice;
    QChartView *m_chartView = nullptr;
};

#endif // REPORT_H
