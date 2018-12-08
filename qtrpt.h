/*
Name: QtRpt
Version: 1.0.4.1
Programmer: Aleksey Osipov
e-mail: aliks-os@yandex.ru
2012-2013
*/

#ifndef QTRPT_H
#define QTRPT_H

#ifdef HAVE_QT5
    #include <QDomDocument>
    #include <QtWidgets/QWidget>
    #include <QtPrintSupport>
#else
    #include <QtGui>
    #include <QtXml/QDomDocument>
#endif

enum BandType {
    ReportTitle,
    PageHeader,
    MasterHeader,
    MasterData,
    MasterFooter,
    ReportSummary,
    PageFooter
};

class QtRPT : public QWidget
{
    Q_OBJECT
public:
    QList<int> recordCount;
    explicit QtRPT(QWidget *parent = 0);    
    bool loadReport(QString fileName);
    void printExec();
    //void setCallbackFunc(void (*func)(int &recNo, QString &paramName, QVariant &paramValue));
    void setBackgroundImage(QPixmap image);

private:
    QPixmap *m_backgroundImage = 0;
    QPainter painter;
    int m_recNo;
    int m_pageReport;
    float koefRes_h;
    float koefRes_w;
    int ph;
    int pw;
    int ml;
    int mr;
    int mt;
    int mb;
    int curPage;
    int totalPage;
    QPrintPreviewWidget *pr;
    QList<QAction *> lst ;
    QDomNode reportTitle,pageHeader,masterData,pageFooter,reportSummary,masterFooter,masterHeader;
    QDomDocument *xmlDoc;
    QDomNode getBand(BandType type, QDomElement docElem);
    void drawField(QDomNode n, int bandTop);
    QString sectionField(QString value);
    QImage sectionFieldImage(QString value);
    QString sectionValue(QString paramName);
    QImage sectionValueImage(QString paramName);
    void newPage(QPrinter *printer, int &y, bool draw);
    void processPHeader(int &y, bool draw);
    void processPFooter(bool draw);
    void processMFooter(QPrinter *printer, int &y, bool draw);
    void processRSummary(QPrinter *printer, int &y, bool draw);
    QColor colorFromString(QString value);
    //void (*callbackFunc)(int &recNo, QString &paramName, QVariant &paramValue);
    void processReport(QPrinter *printer, bool draw, int pageReport);
    void processRTitle(int &y, bool draw);
    void processMHeader(int &y, bool draw);
    void processMasterData(QPrinter *printer, int &y, bool draw, int pageReport);
    void setPageSettings(QPrinter *printer, QDomElement docElem);
    void drawBackground(QPainter &painter);
    bool isFieldVisible(const QDomElement &e);
    QString evaluateExpression(const QString& exp) const;

protected:
    bool eventFilter(QObject *obj, QEvent *e);

signals:
    void setValue(int &recNo, QString &paramName, QVariant &paramValue, int reportPage);
    void setValueImage(int &recNo, QString &paramName, QImage &paramValue, int reportPage);

public slots:
    void printPreview(QPrinter *printer);

};

#endif // QTRPT_H
