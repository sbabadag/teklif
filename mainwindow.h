#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtrpt.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void setValue(int &recNo, QString &paramName, QVariant &paramValue, int reportPage);
    void setValueImage(int &recNo, QString &paramName, QImage &paramValue, int reportPage);


private:
    Ui::MainWindow *ui;
    QtRPT *report;

};

#endif // MAINWINDOW_H
