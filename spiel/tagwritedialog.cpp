#include "tagwritedialog.h"
#include "ui_tagwritedialog.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QIntValidator>
#include "beeper.h"

TagWriteDialog::TagWriteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TagWriteDialog)
{
    ui->setupUi(this);
    ui->edCount->setText("0");
    ui->edCount->setValidator(new QIntValidator(0,100,this));
}

TagWriteDialog::~TagWriteDialog()
{
    delete ui;
}

void TagWriteDialog::setPort(QSerialPort *newPort)
{
    mPort = newPort;
    mPort->clear();
    connect(mPort,&QSerialPort::readyRead,this,&TagWriteDialog::readData);
}

void TagWriteDialog::resetPort()
{
   disconnect(mPort,&QSerialPort::readyRead,this,&TagWriteDialog::readData);
   mPort->clear();
   mPort->write("{\"cmd\":\"read\"}");
}

void TagWriteDialog::readData()
{
    QByteArray data=mPort->readAll();
    mAnswer+=data;
    if (mAnswer.contains("}")){
      QJsonDocument doc = QJsonDocument::fromJson(mAnswer);
      QJsonObject obj = doc.object();
      QString res=obj["res"].toString();
      QString inf=obj["inf"].toString();
      Beeper b;
      if (res=="err")
          b.beep(1000);
      else
      {
          b.beep(5000);
          ui->textEdit->append(inf+" Write success");
      }
      mAnswer.clear();
    }
}



void TagWriteDialog::on_pushButton_2_clicked()
{
    resetPort();
    close();
}


void TagWriteDialog::on_pushButton_clicked()
{
    info=ui->edInfo->text();
    counter=ui->edCount->text().toInt();
    ui->textEdit->append("Ready.. Tap Tag on RFID-Device");
    QJsonObject obj;
    obj.insert("cmd","write");
    obj.insert("info",info);
    obj.insert("counter",counter);
    QJsonDocument doc;
    doc.setObject(obj);
    QByteArray msg = doc.toJson();
    mPort->write(msg+"\r\n");
}

