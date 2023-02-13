#ifndef TAGWRITEDIALOG_H
#define TAGWRITEDIALOG_H

#include <QDialog>
#include <QSerialPort>

namespace Ui {
class TagWriteDialog;
}

class TagWriteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TagWriteDialog(QWidget *parent = nullptr);
    ~TagWriteDialog();

    void setPort(QSerialPort *newPort);
    void resetPort();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::TagWriteDialog *ui;
    QSerialPort* mPort;
    QByteArray mAnswer="";

    QString info;
    int counter{0};

private:
    void readData();

};

#endif // TAGWRITEDIALOG_H
