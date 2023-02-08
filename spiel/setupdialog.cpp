#include "setupdialog.h"
#include "ui_setupdialog.h"


SetupDialog::SetupDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupDialog)
{
    ui->setupUi(this);
    ui->cbPort->clear();
    QList<QSerialPortInfo> pi= QSerialPortInfo::availablePorts();

    foreach(QSerialPortInfo p, pi)
    {
        ui->cbPort->addItem(p.portName());
    }

    QSettings  settings(QSettings::IniFormat, QSettings::UserScope,"OttoPlanspiel", "Unfug");

    ui->cbPort->setCurrentText(settings.value("Terminal/port").value<QString>());


}

SetupDialog::~SetupDialog()
{
    delete ui;
}



void SetupDialog::on_buttonBox_accepted()
{
    QSettings  settings(QSettings::IniFormat, QSettings::UserScope,"OttoPlanspiel", "Unfug");

    settings.setValue("Terminal/port", ui->cbPort->currentText());
}

