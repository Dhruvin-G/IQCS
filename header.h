#ifndef HEADER_H
#define HEADER_H

#include <QWidget>

namespace Ui {
class Header;
}

class Header : public QWidget
{
    Q_OBJECT

public:
    explicit Header(QWidget *parent = nullptr);

    void enbleHomeButton(bool _flag);
    void enbleReportButton(bool _flag);
    ~Header();
signals:
    void logoutButtonClickedSignal();
    void homeButtonClickedSignal();
    void closeButtonClickedSignal();
    void reportButtonClickedSignal();
private slots:
    void on_logoutButtonClicked();
    void on_homeButtonClicked();
    void on_closeButtonClicked();
    void on_reportButtonClicked();
private:
    Ui::Header *ui;
};

#endif // HEADER_H
