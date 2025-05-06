#include "pdfreportgenerator.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <QDebug>

#include <hpdf.h>

PDFReportGenerator::PDFReportGenerator(const QString& jsonFilePath)
    : m_jsonFilePath(jsonFilePath)
{
}

//bool PDFReportGenerator::generatePDF(const QString& outputPath)
//{
//    QFile file(m_jsonFilePath);
//    if (!file.open(QIODevice::ReadOnly)) {
//        qWarning() << "Failed to open JSON file:" << m_jsonFilePath;
//        return false;
//    }

//    QByteArray data = file.readAll();
//    file.close();

//    QJsonParseError parseError;
//    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
//    if (parseError.error != QJsonParseError::NoError) {
//        qWarning() << "JSON parse error:" << parseError.errorString();
//        return false;
//    }

//    QJsonObject root = doc.object();

//    HPDF_Doc pdf = HPDF_New(nullptr, nullptr);
//    if (!pdf) {
//        qWarning() << "Failed to create PDF object.";
//        return false;
//    }

//    HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);

//    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", nullptr);

//    HPDF_Page page = HPDF_AddPage(pdf);
//    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
//    HPDF_Page_SetFontAndSize(page, font, 12);

//    float y = 800;

//    for (const QString& dateKey : root.keys()) {
//        if (y < 100) {
//            page = HPDF_AddPage(pdf);
//            HPDF_Page_SetFontAndSize(page, font, 12);
//            y = 800;
//        }

//        HPDF_Page_BeginText(page);
//        HPDF_Page_TextOut(page, 50, y, QString("Date: " + dateKey).toUtf8().constData());
//        HPDF_Page_EndText(page);
//        y -= 20;

//        QJsonObject shiftObj = root.value(dateKey).toObject();

//        for (int i = 1; i <= 3; ++i) {
//            QString shiftKey = QString("Shift %1").arg(i);
//            if (!shiftObj.contains(shiftKey)) continue;

//            QJsonObject shiftData = shiftObj.value(shiftKey).toObject();
//            int defected = shiftData.value("Defected").toInt();
//            int undefected = shiftData.value("Undefected").toInt();

//            QString line = QString("  %1 - Defected: %2, Undefected: %3")
//                               .arg(shiftKey)
//                               .arg(defected)
//                               .arg(undefected);

//            HPDF_Page_BeginText(page);
//            HPDF_Page_TextOut(page, 70, y, line.toUtf8().constData());
//            HPDF_Page_EndText(page);
//            y -= 20;
//        }

//        y -= 10;
//    }

//    HPDF_SaveToFile(pdf, outputPath.toUtf8().constData());
//    HPDF_Free(pdf);

//    return true;
//}


#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

bool PDFReportGenerator::generatePDF(const QString& outputPath)
{
    // Load JSON file using nlohmann::json
    std::ifstream file(m_jsonFilePath.toStdString());
    if (!file.is_open()) {
        qWarning() << "Failed to open JSON file:" << m_jsonFilePath;
        return false;
    }

    nlohmann::json root;
    try {
        file >> root;
    } catch (const nlohmann::json::parse_error& e) {
        qWarning() << "JSON parse error:" << e.what();
        return false;
    }

    // Initialize PDF generation
    HPDF_Doc pdf = HPDF_New(nullptr, nullptr);
    if (!pdf) {
        qWarning() << "Failed to create PDF object.";
        return false;
    }

    HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", nullptr);
    HPDF_Page page = HPDF_AddPage(pdf);
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    HPDF_Page_SetFontAndSize(page, font, 12);

    float y = 800;

    // Iterate over the JSON data
    for (auto& [dateKey, shiftsArray] : root.items()) {
        if (y < 100) {
            page = HPDF_AddPage(pdf);
            HPDF_Page_SetFontAndSize(page, font, 12);
            y = 800;
        }

        // Print the date
        HPDF_Page_BeginText(page);
        HPDF_Page_TextOut(page, 50, y, QString("Date: " + QString::fromStdString(dateKey)).toUtf8().constData());
        HPDF_Page_EndText(page);
        y -= 20;

        // Iterate over each shift object (1, 2, 3, etc.)
        for (auto& shiftObj : shiftsArray) {
            for (auto& [shiftKey, shiftData] : shiftObj.items()) {
                // Check for defected and undefected values
                int defected = shiftData.value("Defected", 0);
                int undefected = shiftData.value("Undefected", 0);

                QString line = QString("  Shift %1 - Defected: %2, Undefected: %3")
                                   .arg(QString::fromStdString(shiftKey))
                                   .arg(defected)
                                   .arg(undefected);

                HPDF_Page_BeginText(page);
                HPDF_Page_TextOut(page, 70, y, line.toUtf8().constData());
                HPDF_Page_EndText(page);
                y -= 20;
            }
        }

        y -= 10;
    }

    // Save PDF file
    HPDF_SaveToFile(pdf, outputPath.toUtf8().constData());
    HPDF_Free(pdf);

    return true;
}

