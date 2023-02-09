#include "auswertung.h"
#include "ui_auswertung.h"

Auswertung::Auswertung(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Auswertung)
{
    ui->setupUi(this);

}

Auswertung::~Auswertung()
{
    delete ui;
}

void Auswertung::setParts(const QList<Part *> &newParts)
{    
    int minTime{9999}, maxTime{0}, sumTime{0}, count{0};

    foreach (Part* p, newParts)
    {
        int tim=p->timerEnd()-p->timerStart();

        if (p->timerEnd()==0)
            tim=-1;
        ui->txtAuswertung->append("Produktnr: "+p->uID() + "\t"+tr("angefangen")+": " + p->startTime().toString("hh:mm:ss") + "\t"+tr("beendet")+": " + p->endTime().toString("hh:mm:ss") + "\t"+tr("Durchlaufzeit")+": " + QString::number(tim) + " sek.");
        if (tim > 0)
        {
          if (tim < minTime)
            minTime=tim;
          if (tim > maxTime)
            maxTime=tim;
          sumTime+=tim;
          count++;
        }
    }
    ui->txtAuswertung->append("");
    ui->txtAuswertung->append(tr("in Produktion")+": "+QString::number(newParts.size()));
    ui->txtAuswertung->append(tr("angefangen")+": "+QString::number(newParts.size()-count));
    ui->txtAuswertung->append(tr("gefertigt")+": "+QString::number(count));

    if (count > 0)
    {
       ui->txtAuswertung->append("");
       ui->txtAuswertung->append(tr("schnellster Durchlauf")+": "+QString::number(minTime)+ " sek.");
       ui->txtAuswertung->append(tr("langsamster Durchlauf")+": "+QString::number(maxTime)+ " sek.");
       ui->txtAuswertung->append(tr("mittlere Durchlaufzeit")+": "+QString::number(sumTime/count)+ " sek.");
    }
}


