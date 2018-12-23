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
    QString Boyama;
    QString imalatyapimyontemi="Sistemin statik projede belirtilen kalitedeki civatalalı olarak teşkil edilmesi işidir.";
    QString civatabaglantisistemi = "Sistemde bağlantılar 8.8 kalitesinde civatalarla teşkil edilecektir";
    QString isverenyukumlulukleri;
    QString yukleniciyukumlulukleri;
    QString isinfiyati;
    QString odeme;
    QString tmp1,tmp2,tmp3,tmp4;

    TeklifOnYazi = QString("Firmanızın yapacak olduğu %1 kapsamında \n \n fiyat teklifimiz ekte bilgilerinize sunulmuştur."
                   " \n \n \n Firmanıza hizmet etmekten mutluluk duyacağımızı bildirir, çalışmalarınızda başarılar dileriz").arg(Baslik);

    TeklifKonusu = QString("Firmanızca yapımını üstlendiğiniz %1 projesi kapsamında talep edilen \n \n %1, \n \n %2, \n \n %3, \n \n %4").arg(CelikMalzemesiTemini).arg(Imalat).arg(Kumlama).arg(Boyama);



    if (ui->checkBox_14->isChecked())
    {
        tmp1 += ui->lineEdit_20->text() + " mikron "  + "shop primer astar ; \n \n";
    }


    if (ui->checkBox_12->isChecked())
    {
        tmp1 += ui->lineEdit_16->text() + " mikron " + ui->comboBox_4->currentText() +ui->lineEdit_17->text() + " renk astar; \n \n";
    }

    if (ui->checkBox_13->isChecked())
    {
        tmp1 += ui->lineEdit_18->text() + " mikron " + ui->comboBox_5->currentText() +ui->lineEdit_19->text() + " renk sonkat; \n \n ";
    }


    Boyama = tmp1;


 if (paramName == "on")                                         paramValue = Baslik;
 if (paramName == "firmaadi")                                   paramValue = ui->lineEdit_3->text();
 if (paramName == "adres")                                      paramValue = ui->lineEdit_13->text();
 if (paramName == "yetkilikisi")                                paramValue = ui->lineEdit_4->text();
 if (paramName == "teklifonyazi")                               paramValue = TeklifOnYazi;
 if (paramName == "teklifiyazan")                               paramValue = ui->lineEdit_2->text();
 if (paramName == "teklifkonusu")                               paramValue = TeklifKonusu;
 if (paramName == "imalatyapimyontemi")                         paramValue = imalatyapimyontemi;
 if (paramName == "boyasistemi")                                paramValue =Boyama;
 if (paramName == "civatabaglantisistemi")                      paramValue =civatabaglantisistemi;
 if (paramName == "isverenyukumlulukleri")                      paramValue =isverenyukumlulukleri;
 if (paramName == "yukleniciyukumlulukleri")                    paramValue =yukleniciyukumlulukleri;
 if (paramName == "isinsuresi")                                 paramValue ="Yer/Proje Tesliminden  itibaren " + ui->lineEdit_22->text() + " iş günüdür.";
 if (paramName == "isinfiyati")                                 paramValue =ui->lineEdit_14->text()+ "TL/kg" ;
 if (paramName == "odeme")                                      paramValue =odeme;
 if (paramName == "referanslar")                                paramValue =ui->textEdit_2->toPlainText();
//
 if (ui->checkBox->isChecked())
 {
     tmp2 += " - Elektrik temini \n \n ";
 }
//
 if (ui->checkBox_3->isChecked())
 {
     tmp2 += " - Kalacak yer temini \n \n";
 }
//
 if (ui->checkBox_4->isChecked())
 {
     tmp2 += " - Yemek temini \n \n";
 }
//
 if (ui->checkBox_5->isChecked())
 {
     tmp2 += " - Vinç ve taşıma araçları temini \n \n";
 }
//
 if (ui->checkBox_6->isChecked())
 {
     tmp2 += " - Çelik malzemesinin temini (Profil,sac vs.) \n \n";
 }
//
 if (ui->checkBox_7->isChecked())
 {
     tmp2 += " - Boya temini \n \n";
 }
 //
 if (ui->checkBox_8->isChecked())
 {
     tmp2 += " - Civata ve bağlantı elemanları temini \n \n";
 }
//
 if (ui->checkBox_9->isChecked())
 {
     tmp2 += " - Ankraj temini \n \n";
 }
//
 if (ui->checkBox_10->isChecked())
 {
     tmp2 += " - İmalatın yapılması \n \n";
 }
//
 if (ui->checkBox_18->isChecked())
 {
     tmp2 += " - Sahada montajın yapılması \n \n";
 }
//
 if (ui->checkBox_20->isChecked())
 {
     tmp2 += " - Sevkiyatın yapılması \n \n";
 }
//
 if (ui->checkBox_21->isChecked())
 {
     tmp2 += " - Vergiler \n \n";
 }
// End of işveren yükümlülükleri

 if (ui->checkBox_22->isChecked())
 {
     tmp3 += " - Elektrik temini \n \n";
 }
//
 if (ui->checkBox_23->isChecked())
 {
     tmp3 += " - Kalacak yer temini \n \n";
 }
//
 if (ui->checkBox_24->isChecked())
 {
     tmp3 += " - Yemek temini \n \n";
 }
//
 if (ui->checkBox_25->isChecked())
 {
     tmp3 += " - Vinç ve taşıma araçları temini \n \n";
 }
//
 if (ui->checkBox_26->isChecked())
 {
     tmp3 += " - Çelik malzemesinin temini (Profil,sac vs.) \n \n";
 }
//
 if (ui->checkBox_27->isChecked())
 {
     tmp3 += " - Boya temini \n \n";
 }
 //
 if (ui->checkBox_28->isChecked())
 {
     tmp3 += " - Civata ve bağlantı elemanları temini \n \n";
 }
//
 if (ui->checkBox_29->isChecked())
 {
     tmp3 += " - Ankraj temini \n \n";
 }
//
 if (ui->checkBox_30->isChecked())
 {
     tmp3 += " - İmalatın yapılması \n \n";
 }
//
 if (ui->checkBox_31->isChecked())
 {
     tmp3 += " - Sahada montajın yapılması \n \n";
 }
//
 if (ui->checkBox_32->isChecked())
 {
     tmp3 += " - Sevkiyatın yapılması \n \n";
 }
//
 if (ui->checkBox_33->isChecked())
 {
     tmp3 += " - Vergiler \n \n";
 }
//
 if (ui->checkBox_34->isChecked())
 {
     tmp3 += " - İmalathane \n \n";
 }

//

 if (paramName == "isverenyukumlulukleri")                      paramValue = tmp2;
 if (paramName == "yukleniciyukumlulukleri")                    paramValue = tmp3;



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
