#ifndef YSER_H
#define YSER_H
#include "ybase.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QWidget>
#include "stdio.h"
#include <iostream>
#include "ui_widget.h"
#include <QMessageBox>
#include <QInputDialog>
#include "qprocess.h"
namespace Ui {
class Yser;
}
class Yser : public QWidget
{
    Q_OBJECT
public: 
    Ui::Widget *sui;
    explicit Yser(QWidget *parent = 0);
    void Init();
private:

    QButtonGroup *QButtonGroup_ser_sent_AsciiHex;
    QButtonGroup *QButtonGroup_ser_recv_AsciiHex;
    QButtonGroup *QButtonGroup_ser_sent_Utf8Gb23;
    QButtonGroup *QButtonGroup_ser_recv_Utf8Gb23;
    QSerialPort *my_serialport;
    bool bSerEnable;
    bool bSerTimerSentEnable;
    QTimer *serTimerSent;
    QList<int> Aser_baud;
    QStringList Aser_databitsname;
    QList<QSerialPort::DataBits> Aser_dataBits;

    QStringList Aser_stopname;
    QList<QSerialPort::StopBits> Aser_stop;

    QStringList Aser_parityname;
    QList<QSerialPort::Parity> Aser_parity;
signals:   
public slots:
private slots:
    void Close();
    void Scan();
    void SlotTimerSet();
    void Sent();
    void Open();
    void SlotSentAsciiHex();
    void SlotRecvAsciiHex();
    void RecvClear();
    void Recv();
};

#endif // YSER_H
