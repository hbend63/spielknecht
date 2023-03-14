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
    QTime s,e;

    foreach (Part* p, newParts)
    {
        int tim=p->timerEnd()-p->timerStart();

        if (p->timerEnd()==0)
            tim=-1;
        //ui->txtAuswertung->append("Produktnr: "+p->info()+"-"+p->uID() + "\t"+tr("angefangen")+": " + p->startTime().toString("hh:mm:ss") + "\t"+tr("beendet")+": " + p->endTime().toString("hh:mm:ss") + "\t"+tr("Durchlaufzeit")+": " + QString::number(tim) + " sek.");
        s=mGameTime.addSecs(p->timerStart());
        e=mGameTime.addSecs(p->timerEnd());

        ui->txtAuswertung->append("Produktnr: "+p->info()+"-"+p->uID() + "\t"+tr("angefangen")+": " + s.toString("hh:mm:ss") + "\t"+tr("beendet")+": " + e.toString("hh:mm:ss") + "\t"+tr("Durchlaufzeit")+": " + QString::number(tim) + " sek.");
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
       ui->txtAuswertung->append(tr("schnellster Durchlauf")+"\t: "+QString::number(minTime/60.0,'g',3)+ " min.");
       ui->txtAuswertung->append(tr("langsamster Durchlauf")+"\t: "+QString::number(maxTime/60.0,'g',3)+ " min.");
       ui->txtAuswertung->append(tr("mittlere Durchlaufzeit")+"\t: "+QString::number(sumTime/(count*60.0),'g',3)+ " min.");
    }
}

QTime Auswertung::gameTime() const
{
    return mGameTime;
}

void Auswertung::setGameTime(const QTime &newGameTime)
{
    mGameTime = newGameTime;
}


