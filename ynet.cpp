#include "ynet.h"
#include "yser.h"
#include "ybase.h"
#include <QWidget>
#include "widget.h"
#include "stdio.h"
#include <iostream>
#include "ui_widget.h"
#include <QMessageBox>
#include <QInputDialog>
#include "qprocess.h"
Ynet::Ynet(QWidget *parent) : QWidget(parent)
{
}

void Ynet::Init(){
    int i=0;

    bNetTimerSentEnable=false;
    Anet_typename.clear();
    Anet_type.clear();

    Anet_typename<<"TCP_Server"<<"TCP_Client"<<"UDP"<<"UDP_Broadcast";
    Anet_type<<NET_TYPE_TCPS<<NET_TYPE_TCPC<<NET_TYPE_UDP<<NET_TYPE_UDPB;

    for(i=0;i<4;i++){
        nui->comboBox_net_type->addItem(Anet_typename[i],i);
    }

    nui->comboBox_net_type->setCurrentIndex(2);//UDP

    QButtonGroup_net_recv_AsciiHex=new QButtonGroup(this);
    QButtonGroup_net_recv_AsciiHex->setExclusive(true);
    QButtonGroup_net_recv_AsciiHex->addButton(nui->radioButton_net_recv_ascii);
    QButtonGroup_net_recv_AsciiHex->addButton(nui->radioButton_net_recv_hex);
    nui->radioButton_net_recv_ascii->setChecked(true);

    QButtonGroup_net_sent_AsciiHex=new QButtonGroup(this);
    QButtonGroup_net_sent_AsciiHex->setExclusive(true);
    QButtonGroup_net_sent_AsciiHex->addButton(nui->radioButton_net_sent_ascii);
    QButtonGroup_net_sent_AsciiHex->addButton(nui->radioButton_net_sent_hex);
    nui->radioButton_net_sent_ascii->setChecked(true);

    QButtonGroup_net_recv_Utf8Gb23=new QButtonGroup(this);
    QButtonGroup_net_recv_Utf8Gb23->setExclusive(true);
    QButtonGroup_net_recv_Utf8Gb23->addButton(nui->radioButton_net_recv_gb2312);
    QButtonGroup_net_recv_Utf8Gb23->addButton(nui->radioButton_net_recv_utf8);
    nui->radioButton_net_recv_gb2312->setChecked(true);

    QButtonGroup_net_sent_Utf8Gb23=new QButtonGroup(this);
    QButtonGroup_net_sent_Utf8Gb23->setExclusive(true);
    QButtonGroup_net_sent_Utf8Gb23->addButton(nui->radioButton_net_sent_gb2312);
    QButtonGroup_net_sent_Utf8Gb23->addButton(nui->radioButton_net_sent_utf8);
    nui->radioButton_net_sent_gb2312->setChecked(true);

    QButtonGroup_net_anyport=new QButtonGroup(this);
    QButtonGroup_net_anyport->setExclusive(true);
    QButtonGroup_net_anyport->addButton(nui->radioButton_net_localportany);
    QButtonGroup_net_anyport->addButton(nui->radioButton_net_localportblind);
    nui->radioButton_net_localportblind->setChecked(true);

    nui->pushButton_NetTimeSent->setText(tr("打开定时发送"));
    nui->lineEdit_NetTimeSent->setText("1000");
    nui->textEdit_net_sent->textCursor().insertText("YYH 袁一涵");

    nui->lineEdit_net_DestIP->setText("127.0.0.1");
    nui->lineEdit_net_DestPort->setText("61000");
    nui->lineEdit_net_LocalPort->setText("62000");
    Scan();
    connect(QButtonGroup_net_anyport,SIGNAL(buttonClicked(int)),this,SLOT(SlotAnyPort()));
    connect(nui->pushButton_net_scan,SIGNAL(clicked(bool)),this,SLOT(Scan()));
    connect(nui->comboBox_net_type,SIGNAL(currentIndexChanged(int)),this,SLOT(SlotTypeChanged(int)));
    connect(nui->pushButton_NetTimeSent,SIGNAL(clicked(bool)),this,SLOT(SlotTimerSet()));
    connect(nui->pushButton_net_sent,SIGNAL(clicked(bool)),this,SLOT(Sent()));
    connect(nui->pushButton_net_recv_clear,SIGNAL(clicked(bool)),this,SLOT(RecvClear()));
    connect(nui->pushButton_net_open,SIGNAL(clicked(bool)),this,SLOT(Open()));
    connect(QButtonGroup_net_sent_AsciiHex,SIGNAL(buttonClicked(int)),this,SLOT(SlotSentAsciiHex()));
    connect(QButtonGroup_net_recv_AsciiHex,SIGNAL(buttonClicked(int)),this,SLOT(SlotRecvAsciiHex()));
}
void Ynet::Scan(){
    int i=0;
    QHostAddress HOSTaddress;
    QString localHostName = QHostInfo::localHostName();

    qDebug() <<"localHostName:"<<localHostName;
    QHostInfo info = QHostInfo::fromName(localHostName);
    nui->comboBox_net_localIP->clear();
    nui->comboBox_net_localIP->addItem("127.0.0.1",i);
    i++;
    foreach(HOSTaddress,info.addresses())
    {
        if(HOSTaddress.protocol() == QAbstractSocket::IPv4Protocol)
        {
            qDebug() <<"IPV4 Address: "<< HOSTaddress.toString();
            nui->comboBox_net_localIP->addItem(HOSTaddress.toString(),i);
            i++;
        }
    }
    nui->comboBox_net_localIP->setCurrentIndex(0);
    Close();
}
void Ynet::SlotAnyPort(){
    if(nui->radioButton_net_localportany->isChecked()==true){
        nui->lineEdit_net_LocalPort->setEnabled(false);
    }
    if(nui->radioButton_net_localportblind->isChecked()==true){
        nui->lineEdit_net_LocalPort->setEnabled(true);
    }
}
void Ynet::Close(){
    bNetEnable=false;
    //udpsocket->close();
    nui->pushButton_net_open->setText("打开");
}
void Ynet::SlotTypeChanged(int index){
    Close();
    nui->pushButton_net_open->setText("打开");
    switch (index) {
    case NET_TYPE_TCPS://
        nui->lineEdit_net_DestIP->setEnabled(false);
        nui->lineEdit_net_DestPort->setEnabled(false);
        break;
    case NET_TYPE_TCPC://
        nui->lineEdit_net_DestIP->setEnabled(true);
        nui->lineEdit_net_DestPort->setEnabled(true);

        break;
    case NET_TYPE_UDP://
        nui->lineEdit_net_DestIP->setEnabled(true);
        nui->lineEdit_net_DestPort->setEnabled(true);

        break;
    case NET_TYPE_UDPB://
        nui->lineEdit_net_DestIP->setEnabled(true);
        nui->lineEdit_net_DestPort->setEnabled(true);

        break;
    default:
        break;
    }
}
void Ynet::SlotTimerSet(){
    if(bNetTimerSentEnable==false){
        nui->pushButton_NetTimeSent->setText(tr("关闭定时发送"));
        netTimerSent = new QTimer(this);
        connect(netTimerSent, SIGNAL(timeout()), this, SLOT(Sent()));
        netTimerSent->start(nui->lineEdit_NetTimeSent->text().toInt());

        //
        bNetTimerSentEnable=true;
    }else{
        nui->pushButton_NetTimeSent->setText(tr("打开定时发送"));
        netTimerSent->stop();
        bNetTimerSentEnable=false;
    }
}
void Ynet::Sent(){
    if(bNetEnable==true){
        QByteArray ByteSent;
        QByteArray Bytetextedit = nui->textEdit_net_sent->toPlainText().toUtf8();

        if(nui->radioButton_net_sent_ascii->isChecked()==true){
            if(nui->radioButton_net_sent_utf8->isChecked()==true){
                ByteSent=(Bytetextedit);
            }
            if(nui->radioButton_net_sent_gb2312->isChecked()==true){
                ByteSent=  Ybase::ByteUtf8toGb2312((Bytetextedit));
            }
        }
        if(nui->radioButton_net_sent_hex->isChecked()==true){
            ByteSent=Ybase::HexByteArrayToByteArray(Bytetextedit);

        }

        if(nui->checkBox_net_sent_newliner->isChecked()==true){
            ByteSent=ByteSent.append("\r");
        }
        if(nui->checkBox_net_sent_newlinen->isChecked()==true){
            ByteSent=ByteSent.append("\n");
        }

        QHostAddress sentAddress;

        switch (nui->comboBox_net_type->currentIndex()) {
        case NET_TYPE_UDP :
            //向特定IP发送
            sentAddress = QHostAddress(nui->lineEdit_net_DestIP->text());
            udpsocket->writeDatagram(ByteSent, ByteSent.length(),sentAddress,nui->lineEdit_net_DestPort->text().toInt());
            break;
        case NET_TYPE_UDPB :
            //UDP广播
            udpsocket->writeDatagram(ByteSent,ByteSent.length(),QHostAddress::Broadcast,nui->lineEdit_net_DestPort->text().toInt());
            break;

        default:
            break;
        }


    }else{
        int rb
                = QMessageBox::warning(NULL,
                                       tr("警告"),
                                       tr("未打开Socket！请先打开一个Socket"),
                                       QMessageBox::Yes,
                                       QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            qDebug()<<"警告!先打开一个Socket";
        }

    }
}
void Ynet::Open(){

    if(bNetEnable==false){
        switch (nui->comboBox_net_type->currentIndex()) {
        case NET_TYPE_UDP:
            bNetEnable=true;
            nui->pushButton_net_open->setText("关闭UDP");
            udpsocket=new QUdpSocket(this);
            if(nui->radioButton_net_localportblind->isChecked()==true){
                udpsocket->bind(nui->lineEdit_net_LocalPort->text().toInt());
            }
            connect(udpsocket,SIGNAL(readyRead()),this,SLOT(Recv()));
            break;
        case NET_TYPE_UDPB:
            bNetEnable=true;
            nui->pushButton_net_open->setText("关闭UDP_B");
            udpsocket=new QUdpSocket(this);
            if(nui->radioButton_net_localportblind->isChecked()==true){
                udpsocket->bind(nui->lineEdit_net_LocalPort->text().toInt());
            }
            connect(udpsocket,SIGNAL(readyRead()),this,SLOT(Recv()));
            break;
        default:
            break;
        }
    }else{
        Close();

    }
}
void Ynet::Recv(){
    QByteArray requestData;
    QByteArray requestDataNet;
    requestDataNet.clear();
    if(bNetEnable==true){
        while(udpsocket->hasPendingDatagrams()){
            requestDataNet.resize(udpsocket->pendingDatagramSize());
            udpsocket->readDatagram(requestDataNet.data(),requestDataNet.size());
        }
        if(requestDataNet.size()!=0)
        {
            if(nui->radioButton_net_recv_hex->isChecked()==true){
                requestData=Ybase::ByteArrayToHexByteArray(requestDataNet);}
            if(nui->radioButton_net_recv_ascii->isChecked()==true){
                requestData=requestDataNet;
                if(nui->radioButton_net_recv_utf8->isChecked()==true){
                    requestData=requestData;
                }
                if(nui->radioButton_net_recv_gb2312->isChecked()==true){
                    requestData=  Ybase::ByteGb2312toUtf8(requestData);
                }
            }

            if(nui->checkBox_net_recv_newline->isChecked()==true){
                nui->textBrowser_net_recv->append(QString(requestData));//new
            }
            else{
                nui->textBrowser_net_recv->textCursor().insertText(QString(requestData));//add
            }

            nui->textBrowser_net_recv->moveCursor(QTextCursor::End);

        }}
    requestData.clear();

}

void Ynet::SlotRecvAsciiHex(){
    if(nui->radioButton_net_recv_ascii->isChecked()==true){
        nui->radioButton_net_recv_gb2312->setEnabled(true);
        nui->radioButton_net_recv_utf8->setEnabled(true);
    }
    if(nui->radioButton_net_recv_hex->isChecked()==true){
        nui->radioButton_net_recv_gb2312->setEnabled(false);
        nui->radioButton_net_recv_utf8->setEnabled(false);
    }
}

void Ynet::SlotSentAsciiHex(){
    QByteArray ByteEDIT;//=ByteTEXT;
    QByteArray ByteTEXT = (nui->textEdit_net_sent->toPlainText().toUtf8());
    if(nui->radioButton_net_sent_ascii->isChecked()==true){
        if(nui->radioButton_net_sent_utf8->isChecked()==true){
            ByteEDIT= (Ybase::HexByteArrayToByteArray(ByteTEXT));
        }
        if(nui->radioButton_net_sent_gb2312->isChecked()==true){
            ByteEDIT=  Ybase::ByteGb2312toUtf8(Ybase::HexByteArrayToByteArray(ByteTEXT));
        }
        nui->radioButton_net_sent_gb2312->setEnabled(true);
        nui->radioButton_net_sent_utf8->setEnabled(true);
    }
    if(nui->radioButton_net_sent_hex->isChecked()==true){
        if(nui->radioButton_net_sent_utf8->isChecked()==true){
            ByteEDIT=Ybase::ByteArrayToHexByteArray(ByteTEXT);
            //qDebug()<<"utf8:"<<ByteEDIT;
        }
        if(nui->radioButton_net_sent_gb2312->isChecked()==true){
            ByteEDIT= Ybase::ByteArrayToHexByteArray(Ybase::ByteUtf8toGb2312(ByteTEXT));
            //qDebug()<<"gb2312:"<<ByteEDIT;
        }
        nui->radioButton_net_sent_gb2312->setEnabled(false);
        nui->radioButton_net_sent_utf8->setEnabled(false);
    }
    nui->textEdit_net_sent->clear();
    nui->textEdit_net_sent->textCursor().insertText(QString(ByteEDIT));
}
void Ynet::SlotSentUtf8Gb2312(){
    //....//20170619
}
void Ynet::RecvClear(){
    nui->textBrowser_net_recv->clear();
}
