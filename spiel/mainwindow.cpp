#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtAlgorithms>
#include <QSettings>
#include "auswertung.h"
#include "setupdialog.h"
#include "beeper.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{    
    ui->setupUi(this);
    ui->lbUIDDesc->setText(tr("Read UID")+": ");
    ui->lbUID->setText(tr("None"));
    ui->lbRemain->setText(QString::number(mTimeCounter/60)+" min");
    ui->lbStorno->clear();
    port = new QSerialPort;
    setupPort();


    mTimer = new QTimer();
    connect(mTimer, &QTimer::timeout, this, &MainWindow::onTimer);
    mTimer->start(1000);
    onTimer();
}

MainWindow::~MainWindow()
{
    delete port;
    delete ui;
}

void MainWindow::readData()
{
    if (!mIsRunning)
    {
        QByteArray data=port->readAll();
        data.clear();
        return;
    }
     QByteArray data=port->readAll();     
     mAnswer+=data;
     if (mAnswer.contains("\r\n")){
       QJsonDocument doc = QJsonDocument::fromJson(mAnswer);
       QJsonObject obj = doc.object();
       ui->lbUID->setText(obj["UID"].toString());
       Part* p = locate(obj);
       if (p==nullptr)
           addPart(obj);
       else
           buchen(p);
       mAnswer.clear();
     }
}

Part *MainWindow::locate(QJsonObject& obj)
{
  //QApplication::beep();
  Beeper b;
  b.beep();

  for (auto it=mPartList.begin(); it!=mPartList.end(); it++)
  {
      Part* p=*it;
      if (p->uID()==obj["UID"].toString())
          return p;
  }
  return nullptr;
}

bool MainWindow::deletePart(Part *p)
{
    for (auto it=mPartList.begin(); it!=mPartList.end(); it++)
    {
        if (*it==p)
        {
            mPartList.pop_back();            
            return true;
        }
    }
    return false;
}

void MainWindow::addPart(QJsonObject& obj)
{
    Part* p= new Part;
    p->setUID(obj["UID"].toString());
    p->setInfo(obj["PN"].toString());
    p->startProcess();
    p->setTimerStart(mTimeCounter);
    mPartList.append(p);
    std::sort(mPartList.begin(),mPartList.end(),lessObjectUID);
    logStatus(p);
    qDebug() << "new: " << p << " " << p->uID();
}

bool MainWindow::lessObjectUID(const Part *fp_w1, const Part *fp_w2)
{
    return fp_w1->uID() < fp_w2->uID();
}

void MainWindow::buchen(Part *p)
{
  qDebug() << "Found: " << p->uID();
  if (mDoStorno)
      storniere(p);
  else {
  p->stepNext();
  p->setTimerEnd(mTimeCounter);
  logStatus(p);
  }
}

void MainWindow::storniere(Part *p)
{
    p->stepPrior();
    mDoStorno=false;
    writeLog(QTime::currentTime().toString("hh:mm:ss")+" <"+tr("Storniert")+"> "+p->uID());
    if (p->processState()==NEW)
       deletePart(p);
    else
       logStatus(p,true);
    ui->lbStorno->clear();
}

void MainWindow::logStatus(Part* p,bool mark)
{
    QString result ;
    if (mark)
        result+="-->";

    qDebug() <<"l " << p->lastState() << " a" << p->processState();
    switch (p->processState())
    {             
        case NEW: break;
        case RUNNING:  result+=p->startTime().toString("hh:mm:ss")+" "+tr("Lagerabgang")+" "+p->info()+"-"+p->uID(); break;
        case DONE:  if (p->lastState()==DONE)
                      result+=p->endTime().toString("hh:mm:ss")+" "+tr("bereits alle Stationen druchlaufen")+" "+p->info()+"-"+p->uID();
                    else
                      result+=p->endTime().toString("hh:mm:ss")+" "+tr("Fertigstellung")+" "+p->info()+"-"+p->uID()+" in "+QString::number(p->timerEnd()-p->timerStart())+" sek.";

    }
    writeLog(result);
}

void MainWindow::setupPort()
{
    QSettings  settings(QSettings::IniFormat, QSettings::UserScope,"OttoPlanspiel", "Unfug");
    QString portname = settings.value("Terminal/port").value<QString>();

    if (portname=="")
    {
         settings.setValue("Terminal/port", "COM1");
         portname="COM1";
    }

    port->setPortName(portname);
    port->setBaudRate(QSerialPort::Baud115200);
    port->setDataBits(QSerialPort::Data8);
    port->setStopBits(QSerialPort::OneStop);
    port->setParity(QSerialPort::NoParity);

    connect(port,&QSerialPort::readyRead,this,&MainWindow::readData);
    port->open(QSerialPort::ReadOnly);
    if (!port->isReadable())
    {
        qDebug() << "Port not readable";
        writeLog(tr("Kein RFID-Lesegerät gefunden an Port")+": "+ portname);

    } else {
        ui->txtLog->clear();
        writeLog(tr("Port ok."));
    }
}

void MainWindow::writeLog(QString message)
{
    ui->txtLog->moveCursor (QTextCursor::Start);
    ui->txtLog->insertPlainText (message+'\n');
    ui->txtLog->moveCursor (QTextCursor::Start);
}



void MainWindow::onTimer()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm:ss");
    ui->lbClock->setText(text);
    if (mIsRunning) {
        mTimeCounter++;
        /*
        if (mTimeCounter < 0)
        {
           mIsRunning=false;
           ui->btnAbbruch->setEnabled(false);
           ui->btnStart->setEnabled(false);
           ui->btnPause->setEnabled(false);
           ui->btnStorno->setEnabled(false);
           ui->btnAuswertung->setEnabled(true);
           ui->txtLog->append(QTime::currentTime().toString("hh:mm:ss")+" "+tr("Spiel beendet")+".");
        }
        */
        ui->lbRemain->setText(QString::number(mTimeCounter/60)+" min "+QString::number(mTimeCounter-(mTimeCounter/60*60))+" sek");
    }
}


void MainWindow::on_btnStart_clicked()
{
    ui->txtLog->clear();
    foreach (Part* p, mPartList)
       delete p;
    mPartList.clear();
    ui->txtLog->append(QTime::currentTime().toString("hh:mm:ss")+" "+tr("Spiel gestartet")+".");
    ui->btnAuswertung->setEnabled(false);
    ui->btnAbbruch->setEnabled(true);
    ui->btnStart->setEnabled(false);
    ui->btnPause->setEnabled(true);
    ui->btnStorno->setEnabled(true);
    mIsRunning=true;
}


void MainWindow::on_btnPause_clicked()
{
    mIsRunning=!mIsRunning;
    if (mIsRunning)
    {
        ui->btnAbbruch->setEnabled(true);
        ui->btnStart->setEnabled(false);
        ui->btnStorno->setEnabled(true);
        ui->txtLog->moveCursor (QTextCursor::Start);
        ui->txtLog->insertPlainText (QTime::currentTime().toString("hh:mm:ss")+" "+tr("Spielpause beendet")+".\n");
        ui->txtLog->moveCursor (QTextCursor::Start);
    }
    else
    {
        ui->btnAbbruch->setEnabled(false);
        ui->btnStart->setEnabled(false);
        ui->btnStorno->setEnabled(false);
        ui->txtLog->moveCursor (QTextCursor::Start);
        ui->txtLog->insertPlainText (QTime::currentTime().toString("hh:mm:ss")+" "+tr("Spielpause")+".\n");
        ui->txtLog->moveCursor (QTextCursor::Start);
    }
}


void MainWindow::on_btnAbbruch_clicked()
{
    mIsRunning=false;
    ui->btnAbbruch->setEnabled(false);
    ui->btnStart->setEnabled(true);
    ui->btnPause->setEnabled(false);
    ui->btnAuswertung->setEnabled(true);
    ui->txtLog->moveCursor (QTextCursor::Start);
    ui->txtLog->insertPlainText (QTime::currentTime().toString("hh:mm:ss")+" "+tr("Spiel beendet")+".\n");
    ui->txtLog->moveCursor (QTextCursor::Start);
}


void MainWindow::on_btnStorno_clicked()
{
   if (mPartList.size()>0)
   {
     mDoStorno=true;
     ui->lbStorno->setText(tr("warte auf TAG"));
   }
}


void MainWindow::on_btnAuswertung_clicked()
{
    if (mPartList.size()==0) return;
    Auswertung* aw = new Auswertung(this);
    aw->setParts(mPartList);
    aw->exec();
    delete aw;

}


void MainWindow::on_actionSetup_triggered()
{
   SetupDialog *setup=new SetupDialog(this);
   setup->exec();
   delete setup;
   port->close();
   setupPort();
}

