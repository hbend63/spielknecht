#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QSerialPortInfo>
#include <QSettings>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SetupDialog(QWidget *parent = nullptr);
    ~SetupDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SetupDialog *ui;    
};

#endif // SETUPDIALOG_H
