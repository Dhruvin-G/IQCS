#include "report.h"
#include "ui_report.h"
#include<QMessageBox>
#include <QtCharts>


Report::Report(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Report)
{
    ui->setupUi(this);
    setPieChart();
    connect(ui->pushButton, &QPushButton::clicked, this, &Report::on_exportPDFButtonClicked);
}
void Report::on_exportPDFButtonClicked(){
    QMessageBox::information(this, "PDF Export", "PDF is exported successfully");
}

void Report::setPieChart()
{
    QPieSeries *series = new QPieSeries();
    series->append("Defected", 30);
    series->append("Undefected", 200);

    QPieSlice *defectedSlice = series->slices().at(0);
    defectedSlice->setLabelVisible(true);
    defectedSlice->setBrush(Qt::red);

    QPieSlice *undefectedSlice = series->slices().at(1);
    undefectedSlice->setLabelVisible(true);
    undefectedSlice->setBrush(Qt::green);


    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Defect Analysis");
    chart->legend()->setVisible(false);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->gridLayout->replaceWidget(ui->widget,chartView);
    ui->widget->deleteLater();
}
Report::~Report()
{
    delete ui;
}
