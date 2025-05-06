#include "report.h"
#include "ui_report.h"
#include<QMessageBox>
#include<hpdf.h>

#include "jsonhandler.h"
#include "pdfreportgenerator.h"

Report::Report(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Report)
{
    ui->setupUi(this);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setMaximumDate(QDate::currentDate());

    fetchSessionData();
    setPieChart();

    connect(ui->pushButton, &QPushButton::clicked, this, &Report::on_exportPDFButtonClicked);
    connect(ui->dateEdit, &QDateEdit::dateChanged, this, &Report::on_dateChange);
}
void Report::on_exportPDFButtonClicked(){
//    const char* filename = "output.pdf";

//       // Create PDF document
//       HPDF_Doc pdf = HPDF_New(nullptr, nullptr);
//       if (!pdf) {
//           QMessageBox::critical(this, "Error", "Failed to create PDF object.");
//           return;
//       }

//       try {
//           // Add a new page
//           HPDF_Page page = HPDF_AddPage(pdf);

//           // Set page size and font
//           HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A5, HPDF_PAGE_PORTRAIT);
//           HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", nullptr);
//           HPDF_Page_SetFontAndSize(page, font, 14);

//           // Write text
//           HPDF_Page_BeginText(page);
//           HPDF_Page_TextOut(page, HPDF_Page_GetWidth(page) /4, HPDF_Page_GetHeight(page) /4, ui->label->text().toUtf8().constData());
//           HPDF_Page_TextOut(page, HPDF_Page_GetWidth(page) /2, HPDF_Page_GetHeight(page) /2, ui->label_2->text().toUtf8().constData());

//           HPDF_Page_TextOut(page, HPDF_Page_GetWidth(page) /2, HPDF_Page_GetHeight(page) /2, ui->label_3->text().toUtf8().constData());
//           HPDF_Page_TextOut(page, HPDF_Page_GetWidth(page) /4, HPDF_Page_GetHeight(page) /4, ui->label_4->text().toUtf8().constData());
//           HPDF_Page_EndText(page);

//           // Save the PDF
//           HPDF_SaveToFile(pdf, filename);

//       } catch (...) {
//           HPDF_Free(pdf);
//           QMessageBox::critical(this, "Error", "An exception occurred while creating PDF.");
//           return;
//       }

//       HPDF_Free(pdf);
    PDFReportGenerator generator("/home/dhruvin/qt_projects/IQCS/reportData.json");
    if (generator.generatePDF("output/report.pdf")) {
        qDebug() << "PDF generated successfully.";
        QMessageBox::information(this, "Success", "PDF exported successfully!");
    } else {
        qDebug() << "Failed to generate PDF.";
        QMessageBox::information(this, "Failure", "Failed to export PDF!");
    }

}


void Report::setPieChart()
{
    QPieSeries *series = new QPieSeries();
    int defected = ui->label_4->text().toInt();
    qDebug() << "defected: " << defected;
    int undefected = ui->label_2->text().toInt();
    qDebug() << "undefected: " << undefected;

    QChart *chart = new QChart();
    series->append("Defected", defected);
    series->append("Undefected", undefected);

    defectedSlice = series->slices().at(0);
    defectedSlice->setLabelVisible(true);
    defectedSlice->setLabel(QString("Defected : %1").arg(defected));
    defectedSlice->setLabelFont(QFont("Ubuntu",15,580,false));
    defectedSlice->setBrush(QBrush(QColor("#17153B")));

    undefectedSlice = series->slices().at(1);
    undefectedSlice->setLabelVisible(true);
    undefectedSlice->setLabel(QString("Undefected : %1").arg(undefected));
    undefectedSlice->setLabelFont(QFont("Ubuntu",15,580,false));
    undefectedSlice->setBrush(QBrush(QColor("#C8ACD6")));

    chart->addSeries(series);
    chart->legend()->setVisible(false);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("border: 2px solid black");

    ui->gridLayout->replaceWidget(ui->widget,chartView);

    if(ui->widget != nullptr){

        ui->widget->deleteLater();
    }
    ui->widget = chartView;
}

void Report::fetchSessionData()
{
    JsonHandler handler;
    const std::string filePath = "/home/dhruvin/qt_projects/IQCS/reportData.json";  // Update the path if needed

    json data = handler.loadData(filePath);

    QString qdate = QDate::currentDate().toString("dd-MM-yyyy");
    std::string dateKey = qdate.toStdString();

    int totalDefected = 0;
    int totalUndefected = 0;

    if (data.contains(dateKey)) {
        const auto& shifts = data[dateKey];

        for (const auto& shift : shifts) {
            const auto& shiftObj = shift.begin().value();
            totalDefected += shiftObj.value("Defected", 0);
            totalUndefected += shiftObj.value("Undefected", 0);
        }

        ui->label_2->setText(QString::number(totalUndefected));
        ui->label_4->setText(QString::number(totalDefected));
    } else {
        qDebug() << "No data found for today.";
        ui->label_2->setText("0");
        ui->label_4->setText("0");
    }
//    setPieChart();
}
void Report::on_dateChange(QDate date){
    JsonHandler handler;
        const std::string filePath = "/home/dhruvin/qt_projects/IQCS/reportData.json";
        json data = handler.loadData(filePath);

        QString qdate = date.toString("dd-MM-yyyy");
        std::string dateKey = qdate.toStdString();

        int totalDefected = 0;
        int totalUndefected = 0;

        if (data.contains(dateKey)) {
            const auto& shifts = data[dateKey];

            for (const auto& shift : shifts) {
                const auto& shiftObj = shift.begin().value();
                totalDefected += shiftObj.value("Defected", 0);
                totalUndefected += shiftObj.value("Undefected", 0);
            }

            ui->label_2->setText(QString::number(totalUndefected));
            ui->label_4->setText(QString::number(totalDefected));
        } else {
            qDebug() << "No data found for selected date:" << qdate;
            ui->label_2->setText("0");
            ui->label_4->setText("0");
        }

        // Update pie chart after labels are set
        setPieChart();
}

Report::~Report()
{
    delete ui;
}
