#ifndef YNET_H
#define YNET_H

#include <ybase.h>
#include <QWidget>
#include <QtNetwork>
#include "stdio.h"
#include <iostream>
#include "ui_widget.h"
#include <QMessageBox>
#include <QInputDialog>
#include "qprocess.h"

#define NET_TYPE_TCPS 0
#define NET_TYPE_TCPC 1
#define NET_TYPE_UDP 2
#define NET_TYPE_UDPB 3
namespace Ui {
class Ynet;
}
class Ynet : public QWidget
{
    Q_OBJECT
public:
    Ui::Widget *nui;
    explicit Ynet(QWidget *parent = 0);
    void Init();
private:
    QButtonGroup *QButtonGroup_net_anyport;
    QButtonGroup *QButtonGroup_net_sent_AsciiHex;
    QButtonGroup *QButtonGroup_net_recv_AsciiHex;
    QButtonGroup *QButtonGroup_net_sent_Utf8Gb23;
    QButtonGroup *QButtonGroup_net_recv_Utf8Gb23;

    QUdpSocket *udpsocket;

    bool bNetEnable ;
    bool bNetTimerSentEnable;

    QTimer *netTimerSent;


    QStringList Anet_typename;
    QList<int> Anet_type;

signals:

private slots:
    void Close();
    void SlotAnyPort();
    void Scan();
    void SlotTypeChanged(int index);
    void SlotTimerSet();
    void Sent();
    void Open();
    void SlotSentAsciiHex();
    void SlotRecvAsciiHex();
    void SlotSentUtf8Gb2312();
    void RecvClear();
    void Recv();
};

#endif // YNET_H
