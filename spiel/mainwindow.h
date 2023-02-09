#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include <QList>
#include "part.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:    
    void onTimer();

    void on_btnStart_clicked();

    void on_btnPause_clicked();

    void on_btnAbbruch_clicked();

    void on_btnStorno_clicked();

    void on_btnAuswertung_clicked();

    void on_actionSetup_triggered();

private:
    Part* locate(QJsonObject &obj);
    bool deletePart(Part* p);
    void addPart(QJsonObject &obj);
    static bool lessObjectUID(const Part *fp_w1, const Part *fp_w2);
    void buchen(Part* p);
    void storniere(Part* p);
    void logStatus(Part* p, bool mark=false);
    void setupPort();
    void writeLog(QString message);

private:
    Ui::MainWindow *ui;
    QSerialPort *port;
    void readData();
    QByteArray mAnswer;
    QTimer *mTimer;
    QList<Part*> mPartList;    
    Part* mPart;
    int mTimeCounter{0};
    bool mIsRunning{false};
    bool mDoStorno{false};
};
#endif // MAINWINDOW_H
