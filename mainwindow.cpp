#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qtrpt.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
         QString fileName ="C:/qt_projects/teklif/ONYAZI.xml";
         QtRPT *report = new QtRPT(this);
         report->loadReport(fileName);
         QObject::connect(report, SIGNAL(setValue(int&, QString&, QVariant&, int)),this, SLOT(setValue(int&, QString&, QVariant&, int)));
         QObject::connect(report, SIGNAL(setValueImage(int&, QString&, QImage&, int)),this, SLOT(setValueImage(int&, QString&, QImage&, int)));
         report->printExec();
}

void MainWindow::setValue(int &recNo, QString &paramName, QVariant &paramValue, int reportPage)
{

 if (paramName == "on") paramValue = "BİKİLİTAŞ \n ÇELİK KONSTRÜKSİYON \n YAPIM İŞİ";
 if (paramName == "firmaadi") paramValue = ui->lineEdit_3->text();
 if (paramName == "adres") paramValue = ui->lineEdit_13->text();




}

void MainWindow::setValueImage(int &recNo, QString &paramName, QImage &paramValue, int reportPage) {
    if (paramName == "baslik") {
        QImage *image = new QImage("C:/qt_projects/teklif/baslik.jpg");
        paramValue = *image;
    }
        if (paramName == "logo") {
            QImage *image = new QImage("C:/qt_projects/teklif/ortalogo.jpg");
            paramValue = *image;
    }

}
