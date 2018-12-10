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
    QString OnYazi = ui->lineEdit->text();
    QString Baslik = ui->lineEdit->text();;
    QString TeklifOnYazi;
    QString TeklifKonusu;
    QString CelikMalzemesiTemini = "Çelik malzemesinin temini";
    QString Imalat = "çelik yapının imalat (kesme+çatım- delik delme+kaynak+taşlama+temizlik+boya) işleri";
    QString Kumlama = "sa 1/2 derecesinde kumlanması";
    QString Boyama = QString("%1 micron %2 renk astar, %3 micron %4 renk sonkat boyasının yapılması işidir.").arg(ui->lineEdit_16->text()).arg(ui->lineEdit_17->text()).arg(ui->lineEdit_18->text()).arg(ui->lineEdit_19->text());
    QString imalatyapimyontemi;
    QString civatabaglantisistemi = "Sistemde bağlantılar 8.8 kalitesinde civatalarla teşkil edilecektir";
    QString isverenyukumlulukleri;
    QString yukleniciyukumlulukleri;
    QString isinfiyati;
    QString odeme;

    TeklifOnYazi = QString("Firmanızın yapacak olduğu %1 kapsamında \n \n fiyat teklifimiz ekte bilgilerinize sunulmuştur."
                   " \n \n \n Firmanıza hizmet etmekten mutluluk duyacağımızı bildirir, çalışmalarınızda başarılar dileriz").arg(Baslik);

    TeklifKonusu = QString("Firmanızca yapımını üstlendiğiniz %1 projesi kapsamında talep edilen %1, %2, %3, %4").arg(CelikMalzemesiTemini).arg(Imalat).arg(Kumlama).arg(Boyama);


 if (paramName == "on")                                        paramValue = Baslik;
 if (paramName == "firmaadi")                             paramValue = ui->lineEdit_3->text();
 if (paramName == "adres")                                   paramValue = ui->lineEdit_13->text();
 if (paramName == "yetkilikisi")                            paramValue = ui->lineEdit_4->text();
 if (paramName == "teklifonyazi")                        paramValue = TeklifOnYazi;
 if (paramName == "teklifiyazan")                        paramValue = ui->lineEdit_2->text();
 if (paramName == "teklifkonusu")                       paramValue =  TeklifKonusu;
 if (paramName == "imalatyapimyontemi")        paramValue =  imalatyapimyontemi;
 if (paramName == "boyasistemi")                        paramValue =Boyama;
 if (paramName == "civatabaglantisistemi")       paramValue =civatabaglantisistemi;
 if (paramName == "isverenyukumlulukleri")     paramValue =isverenyukumlulukleri;
 if (paramName == "yukleniciyukumlulukleri")   paramValue =yukleniciyukumlulukleri;
 if (paramName == "isinsuresi")                              paramValue =ui->lineEdit_22->text() + " iş günüdür.";
 if (paramName == "isinfiyati")                               paramValue =ui->lineEdit_14->text()+ "TL/kg" ;
 if (paramName == "odeme")                                  paramValue =odeme;
 if (paramName == "referanslar")                           paramValue =ui->textEdit_2->toPlainText();



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
