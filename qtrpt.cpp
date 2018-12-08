/*
Name: QtRpt
Version: 1.0.5
Programmer: Aleksey Osipov
e-mail: aliks-os@yandex.ru
2012-2013
*/

#include "qtrpt.h"
#include "qprintpreviewwidget.h"
#include <QXmlQuery>
#include <QXmlResultItems>

QtRPT::QtRPT(QWidget *parent) : QWidget(parent) {
    xmlDoc = new QDomDocument("Reports");
}

bool QtRPT::loadReport(QString fileName) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    if (!xmlDoc->setContent(&file)) {
        file.close();
        return false;
    }
    file.close();
    return true;
}

QDomNode QtRPT::getBand(BandType type, QDomElement docElem) {
    QString s_type;
    if (type == ReportTitle)    s_type = "ReportTitle";
    if (type == PageHeader)     s_type = "PageHeader";
    if (type == MasterData)     s_type = "MasterData";
    if (type == PageFooter)     s_type = "PageFooter";
    if (type == ReportSummary)  s_type = "ReportSummary";
    if (type == MasterFooter)   s_type = "MasterFooter";
    if (type == MasterHeader)   s_type = "MasterHeader";
    //QDomElement docElem = xmlDoc->documentElement();  //get root element
    //гуляем во всем элементам родителем которых является корневой
    QDomNode n = docElem.firstChild();//.firstChild();
    while(!n.isNull()) {
        QDomElement e = n.toElement(); // try to convert the node to an element.
        if ((!e.isNull()) && (e.tagName() == "ReportBand")) {
            if (e.attribute("type") == s_type)
                return n;
        }
        n = n.nextSibling();
    }
    return n;
}

QColor QtRPT::colorFromString(QString value) {
    QColor color;
    int start; int end;
    start = value.indexOf("(",0,Qt::CaseInsensitive);
    end =   value.indexOf(")",start+1,Qt::CaseInsensitive);
    QString tmp = value.mid(start+1,end-start-1);
    int v = tmp.section(",",0,0).toInt();
    color.setRed(v);
    v = tmp.section(",",1,1).toInt();
    color.setGreen(v);
    v = tmp.section(",",2,2).toInt();
    color.setBlue(v);
    v = tmp.section(",",3,3).toInt();
    color.setAlpha(v);
    return color;
}

void QtRPT::drawField(QDomNode n, int bandTop) {
    //В качестве параметра подается нод бэнда
    QDomNode c = n.firstChild();
    while(!c.isNull()) {
        QDomElement e = c.toElement(); // try to convert the node to an element.
        if ((!e.isNull()) && (e.tagName() == "TContainerField")) {
            if (isFieldVisible(e)) {

                QFont font(e.attribute("fontFamily"),e.attribute("fontSize").toInt());
                font.setBold(e.attribute("fontBold").toInt());
                font.setItalic(e.attribute("fontItalic").toInt());
                font.setUnderline(e.attribute("fontUnderline").toInt());

                painter.setFont(font);
                painter.setPen(Qt::black);

                int left_ = e.attribute("left").toInt()*koefRes_w;
                int top_ = (bandTop+e.attribute("top").toInt())*koefRes_h;
                int width_ = (e.attribute("width").toInt()-1)*koefRes_w;;
                int height_ = e.attribute("height").toInt()*koefRes_h;

                int cor = QFontMetrics(font).height() * koefRes_h;
                QRect textRect(left_, top_-height_, width_, height_);
                textRect.translate(0, cor );

                QPen pen = painter.pen();

                Qt::Alignment al;
                Qt::Alignment alH, alV;
                if (e.attribute("aligmentH") == "hRight")   alH = Qt::AlignRight;
                if (e.attribute("aligmentH") == "hLeft")    alH = Qt::AlignLeft;
                if (e.attribute("aligmentH") == "hCenter")  alH = Qt::AlignHCenter;
                if (e.attribute("aligmentH") == "hJustify") alH = Qt::AlignJustify;
                if (e.attribute("aligmentV") == "vTop")     alV = Qt::AlignTop;
                if (e.attribute("aligmentV") == "vBottom")  alV = Qt::AlignBottom;
                if (e.attribute("aligmentV") == "vCenter")  alV = Qt::AlignVCenter;
                al = alH | alV;  //colorFromString(e.attribute("borderRight"))

                if ( colorFromString(e.attribute("backgroundColor")) != QColor(255,255,255,0))
                    painter.fillRect(left_+1,top_+1,width_-2,height_-2,colorFromString(e.attribute("backgroundColor")));

                //Set border width
                pen.setWidth(e.attribute("borderWidth","1").replace("px","").toInt()*2);
                //Set border style
                QString borderStyle = e.attribute("borderStyle","solid");
                if (borderStyle == "dashed")
                    pen.setStyle(Qt::DashLine);
                else if (borderStyle == "dotted")
                    pen.setStyle(Qt::DotLine);
                else if (borderStyle == "dot-dash")
                    pen.setStyle(Qt::DashDotLine);
                else if (borderStyle == "dot-dot-dash")
                    pen.setStyle(Qt::DashDotDotLine);
                else
                    pen.setStyle(Qt::SolidLine);

                if ( colorFromString(e.attribute("borderTop")) != QColor(255,255,255,0) ) {
                    pen.setColor(colorFromString(e.attribute("borderTop")));
                    painter.setPen(pen);
                    painter.drawLine(left_, top_, left_ + width_, top_);
                }
                if ( colorFromString(e.attribute("borderBottom")) != QColor(255,255,255,0) ) {
                    pen.setColor(colorFromString(e.attribute("borderBottom")));
                    painter.setPen(pen);
                    painter.drawLine(left_, top_ + height_, left_ + width_, top_ + height_);
                }
                if ( colorFromString(e.attribute("borderLeft")) != QColor(255,255,255,0) ) {
                    pen.setColor(colorFromString(e.attribute("borderLeft")));
                    painter.setPen(pen);
                    painter.drawLine(left_, top_, left_, top_ + height_);
                }
                if ( colorFromString(e.attribute("borderRight")) != QColor(255,255,255,0) ) {
                    pen.setColor(colorFromString(e.attribute("borderRight")));
                    painter.setPen(pen);
                    painter.drawLine(left_ + width_, top_, left_ + width_, top_ + height_);
                }

                if (e.attribute("type","label") == "label") { //NOT Proccess if field set as ImageField
                    QString txt = sectionField(e.attribute("value"));
                    pen.setColor(colorFromString(e.attribute("fontColor")));
                    painter.setPen(pen);
                    painter.drawText(left_+10,top_,width_-10,height_, al | Qt::TextDontClip, txt);
                }
                if (e.attribute("type","label") == "labelImage") { //Proccess field as ImageField
                    QImage image = sectionFieldImage(e.attribute("value"));
                    if (!image.isNull()) {
                        QRectF target(left_, top_, width_, height_);
                        painter.drawImage(target,image);
                    }
                }
                if (e.attribute("type","label") == "image") {  //Proccess as static ImageField
                    QByteArray byteArray;
                    byteArray = QByteArray::fromBase64(e.attribute("picture","text").toLatin1());
                    QPixmap pixmap = QPixmap::fromImage(QImage::fromData(byteArray, "PNG"));
                    painter.drawPixmap(left_,top_,width_,height_,pixmap);
                }
            }
        }
        c = c.nextSibling();
    }
}

QString QtRPT::sectionField(QString value) {
    QString tmpStr;
    QStringList res;
    for (int i = 0; i < value.size(); ++i) {
        if (value.at(i) != '[' && value.at(i) != ']')
            tmpStr += value.at(i);
        else {
            if (value.at(i) == ']') {
                tmpStr += value.at(i);
                res << tmpStr;
                tmpStr.clear();
            }
            if (value.at(i) == '[') {
                if (!tmpStr.isEmpty())
                    res << tmpStr;
                tmpStr.clear();
                tmpStr += value.at(i);
            }
        }
    }
    if (!tmpStr.isEmpty()) res << tmpStr;

    tmpStr.clear();
    for (int i = 0; i < res.size(); ++i) {
        if (res.at(i).contains("[") && res.at(i).contains("]"))
            tmpStr += sectionValue(res.at(i));
        else {
            if (res[i].contains("<Date>"))
                res[i] = res[i].replace("<Date>",QDate::currentDate().toString("dd.MM.yyyy") );
            if (res[i].contains("<Time>"))
                res[i] = res[i].replace("<Time>",QTime::currentTime().toString());
            if (res[i].contains("<Page>"))
                res[i] = res[i].replace("<Page>",QString::number(curPage));
            if (res[i].contains("<TotalPages>"))
                res[i] = res[i].replace("<TotalPages>",QString::number(totalPage));

            tmpStr += res.at(i);
        }
    }

    return tmpStr;
}

QImage QtRPT::sectionFieldImage(QString value) {
    return sectionValueImage(value);
}

QString QtRPT::sectionValue(QString paramName) {
    QVariant paramValue;
    paramName.replace("[","");
    paramName.replace("]","");
    //callbackFunc(recNo, paramName, paramValue);
    emit setValue(m_recNo, paramName, paramValue,m_pageReport);
    return paramValue.toString();
}

QImage QtRPT::sectionValueImage(QString paramName) {
    QImage *paramValue;
    paramName.replace("[","");
    paramName.replace("]","");
    paramValue = new QImage(500,500,QImage::Format_ARGB32);
    emit setValueImage(m_recNo, paramName, *paramValue,m_pageReport);
    return *paramValue;
}

/*void QtRPT::setCallbackFunc( void (*func)(int &recNo, QString &paramName, QVariant &paramValue) ) {
    callbackFunc=func;
}*/

void QtRPT::printExec() {
#ifndef QT_NO_PRINTER
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this, Qt::Window);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
    //preview.setWindowState(Qt::WindowMaximized); //Qt BUG https://bugreports.qt-project.org/browse/QTBUG-14517
    QRect geom = QApplication::desktop()->availableGeometry();
    geom.setTop(30);
    geom.setLeft(5);
    geom.setHeight(geom.height()-6);
    geom.setWidth(geom.width()-6);
    preview.setGeometry(geom);
    //connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
    pr = preview.findChild<QPrintPreviewWidget *>();
    lst = preview.findChildren<QAction *>();
    pr->installEventFilter(this);
    //curPage = 1;
    preview.exec();
#endif
}

void QtRPT::printPreview(QPrinter *printer) {
#ifdef QT_NO_PRINTER
    Q_UNUSED(printer);
#else
    painter.begin(printer);

    /*Make a two pass report
     *First pass calculate total pages
     *Second pass draw a report
     */
    curPage = 1;
    for (int i = 0; i < xmlDoc->documentElement().childNodes().count(); i++) {
        m_pageReport = i;
        //First pass
        processReport(printer,false,i);
        totalPage = curPage;
    }

    curPage = 1;
    for (int i = 0; i < xmlDoc->documentElement().childNodes().count(); i++) {
        m_pageReport = i;
        //Second pass
        processReport(printer,true,i);
    }

    painter.end();
    //pr->setWindowState(pr->windowState() ^ Qt::WindowFullScreen);
#endif
}

void QtRPT::setPageSettings(QPrinter *printer, QDomElement docElem) {
    ph = docElem.attribute("pageHeight").toInt();
    pw = docElem.attribute("pageWidth").toInt();
    ml = docElem.attribute("marginsLeft").toInt();
    mr = docElem.attribute("marginsRight").toInt();
    mt = docElem.attribute("marginsTop").toInt();
    mb = docElem.attribute("marginsBottom").toInt();
    printer->setPageMargins(ml/4+0.01, mt/4+0.01, mr/4+0.01, mb/4+0.01, QPrinter::Millimeter);

    int orientation = docElem.attribute("orientation").toInt();
    if (orientation == 1) {
        pw = docElem.attribute("pageHeight").toInt();
        ph = docElem.attribute("pageWidth").toInt();
        printer->setOrientation(QPrinter::Landscape);
    } else {
        printer->setOrientation(QPrinter::Portrait);
    }

    QRect r = printer->pageRect();
    //painter.drawRect(0,0,r.width(),r.height());   //Рамка вокруг страницы
    koefRes_h = static_cast<double>(r.height()) / (ph - mt - mb);
    koefRes_w = static_cast<double>(r.width())  / (pw - ml - mr);
    reportTitle = getBand(ReportTitle, docElem);
    pageHeader = getBand(PageHeader, docElem);
    pageFooter = getBand(PageFooter, docElem);
    masterFooter = getBand(MasterFooter, docElem);
    reportSummary = getBand(ReportSummary, docElem);
    masterData = getBand(MasterData, docElem);
    masterHeader = getBand(MasterHeader, docElem);
}

void QtRPT::processReport(QPrinter *printer, bool draw, int pageReport) {
    QDomElement docElem = xmlDoc->documentElement().childNodes().at(pageReport).toElement();
    setPageSettings(printer, docElem);
    int y = 0;

    drawBackground(painter);
    if (pageReport > 0) {
        newPage(printer, y, draw);
    }

    processRTitle(y,draw);
    processPHeader(y,draw);
    processMHeader(y,draw);
    processPFooter(draw);


    processMasterData(printer,y,draw,pageReport);
    processMFooter(printer,y,draw);

    processRSummary(printer,y,draw);
    //proccessPFooter(y,draw);
}

bool QtRPT::eventFilter(QObject *obj, QEvent *e) {
    if (obj == pr && e->type()==QEvent::Show)  {
        for (int i = 0; i < lst.size(); i++) {
            if (lst.at(i)->text().contains("Previous page", Qt::CaseInsensitive))
                lst.at(i)->trigger();
        }

        pr->setCurrentPage(0);

        return true;
    }
    return QWidget::eventFilter(obj,e);
}

void QtRPT::newPage(QPrinter *printer, int &y, bool draw) {
    if (draw) {
        printer->newPage();
        drawBackground(painter);
    }
    curPage += 1;
    y = 0;
    processPHeader(y,draw);
    processPFooter(draw);
}

void QtRPT::setBackgroundImage(QPixmap image) {
    m_backgroundImage = &image;
}

void QtRPT::drawBackground(QPainter &painter) {
    if (m_backgroundImage != 0) {
        painter.drawPixmap(-ml*koefRes_w,
                           -mt*koefRes_h,
                           pw*koefRes_w,
                           ph*koefRes_h, *m_backgroundImage);
    }
}

bool QtRPT::isFieldVisible(const QDomElement &e)
{
    QString condition = e.attribute("showIf");
    if (condition.isNull())
        return true;
    condition = sectionField(condition);
    return evaluateExpression(condition) == "true";
}

QString QtRPT::evaluateExpression(const QString &exp) const
{
    // Use XQuery as an expression evaluator since it
    // is built in although it would probably be more efficient to
    // use something else.
    QXmlQuery query;
    // QXMLQuery wants to return a node so need to add tags
    query.setQuery(QString("<e>{%1}</e>").arg(exp));
    if (query.isValid()) {
        QString result;
        query.evaluateTo(&result);
        // strip tags and the newline that gets added to result
        return result.mid(3, result.length() - 8);
    }
    return QString();
}

void QtRPT::processMasterData(QPrinter *printer, int &y, bool draw, int pageReport) {
    if (recordCount.size() > 0) {
        if (pageReport < recordCount.size() && recordCount.at(pageReport) > 0) {
            if (!masterData.isNull()) {
                for (int i = 0; i < recordCount.at(pageReport); i++) {
                    m_recNo = i;

                    int yPF = 0;
                    if (!pageFooter.isNull()) {
                        QDomElement pf = pageFooter.toElement();
                        yPF = pf.attribute("height").toInt();
                    }

                    int yMF = 0;
                    if (!masterFooter.isNull()) {
                        QDomElement mf = masterFooter.toElement();
                        yMF = mf.attribute("height").toInt();
                    }

                    QDomElement e = masterData.toElement();
                    if (y + e.attribute("height").toInt() > ph-mb-mt-yPF-yMF) {
                        newPage(printer, y, draw);
                        processMHeader(y,draw);
                    }

                    if (draw) drawField(masterData,y);
                    y += e.attribute("height").toInt();
                    //painter.drawLine(0,y*koefRes_h,r.width(),y*koefRes_h);
                }
            }
        }
    }
}

void QtRPT::processMHeader(int &y, bool draw) {
    if (masterHeader.isNull()) return;
    QDomElement e = masterHeader.toElement();
    if (draw) drawField(masterHeader,y);
    y += e.attribute("height").toInt();
    //painter.drawLine(0,y*koefRes_h,r.width(),y*koefRes_h);
}

void QtRPT::processRTitle(int &y, bool draw) {
    if (reportTitle.isNull()) return;
    QDomElement e = reportTitle.toElement();
    if (draw) drawField(reportTitle,y);
    y += e.attribute("height").toInt();
    //painter.drawLine(0,y*koefRes_h,r.width(),y*koefRes_h);
}

void QtRPT::processPHeader(int &y, bool draw) {
    if (pageHeader.isNull()) return;
    QDomElement e = pageHeader.toElement();
    if (draw) drawField(pageHeader,y);
    y += e.attribute("height").toInt();
    //painter.drawLine(0,y*koefRes_h,pw*koefRes_h,y*koefRes_h);
}

void QtRPT::processMFooter(QPrinter *printer, int &y, bool draw) {
    if (masterFooter.isNull()) return;
    QDomElement e = masterFooter.toElement();
    if (y + e.attribute("height").toInt() > ph-mb-mt-e.attribute("height").toInt())
        newPage(printer, y, draw);
    if (draw) drawField(masterFooter,y);
    y += e.attribute("height").toInt();
}

void QtRPT::processPFooter(bool draw) {
    if (pageFooter.isNull()) return;
    QDomElement e = pageFooter.toElement();
    int y1 = ph-mb-mt-e.attribute("height").toInt();
    if (draw) {
        drawField(pageFooter,y1);
    }
    //painter.drawLine(0,y1*koefRes_h,pw*koefRes_h,y1*koefRes_h);
}

void QtRPT::processRSummary(QPrinter *printer, int &y, bool draw) {
    if (reportSummary.isNull()) return;
    QDomElement e = reportSummary.toElement();
    if (y + e.attribute("height").toInt() > ph-mb-mt-e.attribute("height").toInt())
        newPage(printer, y, draw);
    if (draw) drawField(reportSummary,y);
    y += e.attribute("height").toInt();
    //painter.drawLine(0,y*koefRes_h,pw*koefRes_h,y*koefRes_h);
}


