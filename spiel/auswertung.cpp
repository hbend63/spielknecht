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
        ui->txtAuswertung->append("Produkt: "+p->uID() + "\tangefangen: " + p->processStart().toString("hh:mm:ss") + "\tbeendet: " + p->processReady().toString("hh:mm:ss") + "\tDurchlaufzeit: " + QString::number(tim) + " sek.");
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
    ui->txtAuswertung->append("in Produktion: "+QString::number(newParts.size()));
    ui->txtAuswertung->append("angefangen: "+QString::number(newParts.size()-count));
    ui->txtAuswertung->append("gefertigt: "+QString::number(count));

    if (count > 0)
    {
       ui->txtAuswertung->append("");
       ui->txtAuswertung->append("schnellster Durchlauf: "+QString::number(minTime)+ " sek.");
       ui->txtAuswertung->append("langsamsamster Durchlauf: "+QString::number(maxTime)+ " sek.");
       ui->txtAuswertung->append("mittlere Durchlaufzeit: "+QString::number(sumTime/count)+ " sek.");
    }
}


