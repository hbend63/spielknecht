#ifndef AUSWERTUNG_H
#define AUSWERTUNG_H

#include <QDialog>
#include <QList>
#include <QTime>
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

    QTime gameTime() const;
    void setGameTime(const QTime &newGameTime);

private:
    Ui::Auswertung *ui;
    QTime mGameTime;


};

#endif // AUSWERTUNG_H
