#ifndef AUSWERTUNG_H
#define AUSWERTUNG_H

#include <QDialog>
#include <QList>
#include "part.h"

namespace Ui {
class Auswertung;
}

class Auswertung : public QDialog
{
    Q_OBJECT

public:
    explicit Auswertung(QWidget *parent = nullptr);
    ~Auswertung();

    void setParts(const QList<Part *> &newParts);

private:
    Ui::Auswertung *ui;

};

#endif // AUSWERTUNG_H
