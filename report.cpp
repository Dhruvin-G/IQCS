#include "report.h"
#include "ui_report.h"
#include<QMessageBox>
#include <QtCharts>
#include<hpdf.h>
Report::Report(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Report)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setMaximumDate(QDate::currentDate());
    setPieChart();
    connect(ui->pushButton, &QPushButton::clicked, this, &Report::on_exportPDFButtonClicked);
}
void Report::on_exportPDFButtonClicked(){
//    QMessageBox::information(this, "PDF Export", "PDF is exported successfully");
    const char* filename = "output.pdf";
//       QString data = ui->lineEdit->text();

       // Create PDF document
       HPDF_Doc pdf = HPDF_New(nullptr, nullptr);
       if (!pdf) {
           QMessageBox::critical(this, "Error", "Failed to create PDF object.");
           return;
       }

       try {
           // Add a new page
           HPDF_Page page = HPDF_AddPage(pdf);

           // Set page size and font
           HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A5, HPDF_PAGE_PORTRAIT);
           HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", nullptr);
           HPDF_Page_SetFontAndSize(page, font, 14);

           // Write text
           HPDF_Page_BeginText(page);
           HPDF_Page_TextOut(page, 50, HPDF_Page_GetHeight(page) - 30, "gvbjhcdgsfbfnjhcfd");
           HPDF_Page_TextOut(page, 50, HPDF_Page_GetHeight(page) - 40, QString::fromStdString("2w1314225437").toUtf8().constData());
           HPDF_Page_EndText(page);

           // Save the PDF
           HPDF_SaveToFile(pdf, filename);
           QMessageBox::information(this, "Success", "PDF exported successfully!");

       } catch (...) {
           HPDF_Free(pdf);
           QMessageBox::critical(this, "Error", "An exception occurred while creating PDF.");
           return;
       }

       HPDF_Free(pdf);
}

void Report::setPieChart()
{
    QPieSeries *series = new QPieSeries();
    int defected = 30;
    int undefected = 200;
    series->append("Defected", defected);
    series->append("Undefected", undefected);

    QPieSlice *defectedSlice = series->slices().at(0);
    defectedSlice->setLabelVisible(true);
    defectedSlice->setLabel(QString("Defected : %1").arg(defected));
    defectedSlice->setLabelFont(QFont("Ubuntu",25,580,false));
    defectedSlice->setBrush(QBrush(QColor("#17153B")));

    QPieSlice *undefectedSlice = series->slices().at(1);
    undefectedSlice->setLabelVisible(true);
    undefectedSlice->setLabel(QString("Undefected : %1").arg(undefected));
    undefectedSlice->setLabelFont(QFont("Ubuntu",25,580,false));
    undefectedSlice->setBrush(QBrush(QColor("#C8ACD6")));


    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->setVisible(false);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("border: 2px solid black");

    ui->gridLayout->replaceWidget(ui->widget,chartView);
    ui->widget->deleteLater();
}
Report::~Report()
{
    delete ui;
}
