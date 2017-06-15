#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore>
#include <QMessageBox>
#include <QInputDialog>
#include "time.h"
#include <QtNetwork>
#include <QButtonGroup>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    int DataInit();
    QByteArray ByteUtf8toGb2312(QByteArray ByteUtf8);
    QByteArray ByteGb2312toUtf8(QByteArray ByteGb2312);
    QByteArray HexByteArrayToByteArray(QByteArray hexByteArray);
    QByteArray ByteArrayToHexByteArray(QByteArray byteArray);
private slots:
    void on_pushButton_ser_open_clicked();
    void SerRecv();

    void on_pushButton_ser_sent_clicked();

    void on_pushButton_ser_close_clicked();

    void on_pushButton_ser_scan_clicked();

    void on_radioButton_ser_sent_ascii_clicked();

    void on_radioButton_ser_sent_hex_clicked();

    void on_radioButton_ser_recv_ascii_clicked();

    void on_radioButton_ser_recv_hex_clicked();

private:

    Ui::Widget *ui;
    QButtonGroup *QButtonGroup_sent_AsciiHex;
    QButtonGroup *QButtonGroup_recv_AsciiHex;
    QButtonGroup *QButtonGroup_sent_Utf8Gb23;
    QButtonGroup *QButtonGroup_recv_Utf8Gb23;
    QSerialPort *my_serialport;
    QByteArray requestData;
    QTimer *timer;
    bool SerEnable;
    QList<int> Aser_baud;
    QStringList Aser_databitsname;
    QList<QSerialPort::DataBits> Aser_dataBits;

    QStringList Aser_stopname;
    QList<QSerialPort::StopBits> Aser_stop;

    QStringList Aser_parityname;
    QList<QSerialPort::Parity> Aser_parity;

};

#endif // WIDGET_H
