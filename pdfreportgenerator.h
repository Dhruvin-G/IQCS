#ifndef PDFREPORTGENERATOR_H
#define PDFREPORTGENERATOR_H

#include <QString>

class PDFReportGenerator
{
public:
    PDFReportGenerator(const QString& jsonFilePath);
    bool generatePDF(const QString& outputPath);

private:
    QString m_jsonFilePath;
};

#endif // PDFREPORTGENERATOR_H
