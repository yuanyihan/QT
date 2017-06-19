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

#define COLOR_BLACK QColor(0,0,0)

#define COLOR_GREEN QColor(0,255,0)
#define COLOR_RED QColor(255,0,0)
#define COLOR_BLUE QColor(0,0,255)

#define COLOR_YELLOW QColor(255,255,0)
#define COLOR_MAGENTA QColor(255,0,255)
#define COLOR_CYAN QColor(0,255,255)

#define NET_TYPE_TCPS 0
#define NET_TYPE_TCPC 1
#define NET_TYPE_UDP 2
#define NET_TYPE_UDPB 3


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    Ui::Widget *ui;
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
    void NetRecv();
    void fSerSent();
    void fNetSent();
    void fNetClose();
    void buttonGroup_netAnyPort(int id);
    void on_pushButton_ser_sent_clicked();

    void on_pushButton_ser_close_clicked();

    void on_pushButton_ser_scan_clicked();

    void on_radioButton_ser_sent_ascii_clicked();

    void on_radioButton_ser_sent_hex_clicked();

    void on_radioButton_ser_recv_ascii_clicked();

    void on_radioButton_ser_recv_hex_clicked();

    void on_pushButton_ser_recv_clear_clicked();

    void on_pushButton_calc_clicked();

    void on_pushButton_SerTimeSent_clicked();

    void on_pushButton_net_scan_clicked();

    void on_comboBox_net_type_currentIndexChanged(int index);

    void on_pushButton_NetTimeSent_clicked();

    void on_pushButton_net_sent_clicked();

    void on_pushButton_net_open_clicked();

    void on_pushButton_net_recv_clear_clicked();

    void on_radioButton_net_sent_ascii_clicked();

    void on_radioButton_net_sent_hex_clicked();

    void on_radioButton_net_recv_ascii_clicked();

    void on_radioButton_net_recv_hex_clicked();

private:


    QButtonGroup *QButtonGroup_ser_sent_AsciiHex;
    QButtonGroup *QButtonGroup_ser_recv_AsciiHex;
    QButtonGroup *QButtonGroup_ser_sent_Utf8Gb23;
    QButtonGroup *QButtonGroup_ser_recv_Utf8Gb23;

    QButtonGroup *QButtonGroup_net_anyport;
    QButtonGroup *QButtonGroup_net_sent_AsciiHex;
    QButtonGroup *QButtonGroup_net_recv_AsciiHex;
    QButtonGroup *QButtonGroup_net_sent_Utf8Gb23;
    QButtonGroup *QButtonGroup_net_recv_Utf8Gb23;
    QSerialPort *my_serialport;
    QUdpSocket *udpsocket;
    bool bSerEnable;
    bool bSerTimerSentEnable;
    bool bNetEnable ;
    bool bNetTimerSentEnable;
    QTimer *serTimerSent;
    QTimer *netTimerSent;
    QList<int> Aser_baud;
    QStringList Aser_databitsname;
    QList<QSerialPort::DataBits> Aser_dataBits;

    QStringList Aser_stopname;
    QList<QSerialPort::StopBits> Aser_stop;

    QStringList Aser_parityname;
    QList<QSerialPort::Parity> Aser_parity;

    QStringList Anet_typename;
    QList<int> Anet_type;

};

#endif // WIDGET_H
