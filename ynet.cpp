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

    nui->checkBox_net_LocalPortBlind->setChecked(true);
    //nui->lineEdit_net_LocalPort->setEnabled(false);
    nui->pushButton_NetTimeSent->setText(tr("打开定时发送"));
    nui->lineEdit_NetTimeSent->setText("1000");
    nui->textEdit_net_sent->textCursor().insertText("YYH 袁一涵");

    nui->lineEdit_net_DestPort->setText("61000");
    nui->lineEdit_net_LocalPort->setText("62000");
    iNetTypeEnable=NET_TYPE_NULL;
    connect(nui->pushButton_net_open,SIGNAL(clicked(bool)),this,SLOT(Open()));
    connect(nui->pushButton_net_sent,SIGNAL(clicked(bool)),this,SLOT(Sent()));
    connect(nui->pushButton_net_scan,SIGNAL(clicked(bool)),this,SLOT(Scan()));
    connect(nui->pushButton_net_recv_clear,SIGNAL(clicked(bool)),this,SLOT(RecvClear()));
    connect(nui->pushButton_NetTimeSent,SIGNAL(clicked(bool)),this,SLOT(SlotTimerSet()));

    connect(nui->checkBox_net_LocalPortBlind,SIGNAL(clicked(bool)),this,SLOT(SlotAnyPort()));

    connect(nui->comboBox_net_type,SIGNAL(currentIndexChanged(int)),this,SLOT(SlotTypeChanged()));
    connect(QButtonGroup_net_sent_AsciiHex,SIGNAL(buttonClicked(int)),this,SLOT(SlotSentAsciiHex()));
    connect(QButtonGroup_net_recv_AsciiHex,SIGNAL(buttonClicked(int)),this,SLOT(SlotRecvAsciiHex()));
    SlotTypeChanged();
    Scan();
    nui->comboBox_net_localIP->setCurrentIndex(nui->comboBox_net_localIP->count()-1);
    nui->lineEdit_net_DestIP->setText(nui->comboBox_net_localIP->currentText());
}
void Ynet::Scan(){
    int i=0;
    Close();
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
}
void Ynet::SlotAnyPort(){
    if(nui->checkBox_net_LocalPortBlind->isChecked()==false){
        nui->lineEdit_net_LocalPort->setEnabled(false);
    }else{
        nui->lineEdit_net_LocalPort->setEnabled(true);
    }
}
void Ynet::Close(){
    if(iNetTypeEnable!=NET_TYPE_NULL){
        switch (iNetTypeEnable) {
        //case NET_TYPE_TCPS_LISTEN:

        case NET_TYPE_TCPS://
            qDebug() <<"TCPS Disconnect:"<<tcpsocket->peerAddress().toString()<<":"<<QString::number(tcpsocket->peerPort());
            tcpsocket->disconnectFromHost();
            tcpserver->deleteLater();
            tcpsocket->deleteLater();
            disconnect(tcpsocket,SIGNAL(readyRead()),this,SLOT(Recv()));
            break;
        case NET_TYPE_TCPC://
            //case NET_TYPE_TCPC_CONNECTING:
            qDebug() <<"TCPC Disconnect:"<<tcpsocket->peerAddress().toString()<<":"<<QString::number(tcpsocket->peerPort());
            tcpsocket->disconnect();
            tcpsocket->deleteLater();
            break;
        case NET_TYPE_UDP://
            udpsocket->close();
            udpsocket->deleteLater();
            break;
        case NET_TYPE_UDPB://
            udpsocket->close();
            udpsocket->deleteLater();
            break;
        default:
            break;
        }

        qDebug() <<"Close: "<<Anet_typename.at(iNetTypeEnable);
    }
    iNetTypeEnable=NET_TYPE_NULL;
    nui->pushButton_net_open->setText("打开");
    netAddrNow=NULL;
    netPortNow=0;

}
void Ynet::SlotTypeChanged(){

    Close();

    //NET_TYPE
    int index;
    index=nui->comboBox_net_type->currentIndex();
    switch (index) {
    case NET_TYPE_TCPS://
        nui->lineEdit_net_DestIP->setEnabled(false);
        nui->lineEdit_net_DestPort->setEnabled(false);
        nui->checkBox_net_LocalPortBlind->setChecked(true);
        nui->checkBox_net_LocalPortBlind->setEnabled(false);
        nui->checkBox_net_DestFollow->setChecked(true);
        nui->checkBox_net_DestFollow->setEnabled(false);
        break;
    case NET_TYPE_TCPC://
        nui->lineEdit_net_DestIP->setEnabled(true);
        nui->lineEdit_net_DestPort->setEnabled(true);
        nui->checkBox_net_LocalPortBlind->setChecked(true);
        nui->checkBox_net_LocalPortBlind->setEnabled(false);
        nui->checkBox_net_DestFollow->setChecked(false);
        nui->checkBox_net_DestFollow->setEnabled(false);
        break;
    case NET_TYPE_UDP://
        nui->lineEdit_net_DestIP->setEnabled(true);
        nui->lineEdit_net_DestPort->setEnabled(true);
        nui->checkBox_net_LocalPortBlind->setEnabled(true);
        nui->checkBox_net_DestFollow->setChecked(false);
        nui->checkBox_net_DestFollow->setEnabled(true);
        break;
    case NET_TYPE_UDPB://
        nui->lineEdit_net_DestIP->setEnabled(true);
        nui->lineEdit_net_DestPort->setEnabled(true);
        nui->checkBox_net_LocalPortBlind->setEnabled(true);
        nui->checkBox_net_DestFollow->setChecked(false);
        nui->checkBox_net_DestFollow->setEnabled(true);
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
        qDebug()<<"netTimerSent->stop()";
    }
}
void Ynet::Sent(){
    if(iNetTypeEnable!=NET_TYPE_NULL){
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

        switch (iNetTypeEnable) {
        case NET_TYPE_UDP :
            //向特定IP发送
            sentAddress = QHostAddress(nui->lineEdit_net_DestIP->text());
            udpsocket->writeDatagram(ByteSent, ByteSent.length(),sentAddress,nui->lineEdit_net_DestPort->text().toInt());
            break;
        case NET_TYPE_UDPB :
            //UDP广播
            udpsocket->writeDatagram(ByteSent,ByteSent.length(),QHostAddress::Broadcast,nui->lineEdit_net_DestPort->text().toInt());
            break;
        case NET_TYPE_TCPS :
        case NET_TYPE_TCPC :
            //向特定IP发送
            tcpsocket->write(ByteSent,ByteSent.length());
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
            if(bNetTimerSentEnable==true){
                nui->pushButton_NetTimeSent->setText(tr("打开定时发送"));
                netTimerSent->stop();
                bNetTimerSentEnable=false;
                qDebug()<<"netTimerSent->stop()";
            }
        }

    }
}
void Ynet::Open(){
    static bool FlagOpenClose=true;
    netAddrNow=NULL;
    netPortNow=0;
    if((FlagOpenClose==true)){
        switch (nui->comboBox_net_type->currentIndex()) {
        case NET_TYPE_UDP:
            iNetTypeEnable=nui->comboBox_net_type->currentIndex();
            nui->pushButton_net_open->setText("关闭UDP");
            udpsocket=new QUdpSocket(this);
            if(nui->checkBox_net_LocalPortBlind->isChecked()==true){
                udpsocket->bind(nui->lineEdit_net_LocalPort->text().toInt(),QAbstractSocket::ShareAddress|QAbstractSocket::ReuseAddressHint);
                qDebug() <<"blind: "<<nui->lineEdit_net_LocalPort->text();
            }
            connect(udpsocket,SIGNAL(readyRead()),this,SLOT(Recv()));
            FlagOpenClose=false;
            qDebug() <<"Open: "<<Anet_typename.at(iNetTypeEnable);
            break;
        case NET_TYPE_UDPB:
            iNetTypeEnable=nui->comboBox_net_type->currentIndex();
            nui->pushButton_net_open->setText("关闭UDP_B");
            udpsocket=new QUdpSocket(this);
            if(nui->checkBox_net_LocalPortBlind->isChecked()==true){
                udpsocket->bind(nui->lineEdit_net_LocalPort->text().toInt(),QAbstractSocket::ShareAddress|QAbstractSocket::ReuseAddressHint);
                qDebug() <<"blind: "<<nui->lineEdit_net_LocalPort->text();
            }
            connect(udpsocket,SIGNAL(readyRead()),this,SLOT(Recv()));
            FlagOpenClose=false;
            qDebug() <<"Open: "<<Anet_typename.at(iNetTypeEnable);
            break;
        case NET_TYPE_TCPC://
            //iNetTypeEnable=nui->comboBox_net_type->currentIndex();
            tcpsocket=new QTcpSocket(this);
            tcpsocket->abort(); //取消已有的连接
            tcpsocket->connectToHost(nui->lineEdit_net_DestIP->text(),nui->lineEdit_net_DestPort->text().toInt());

            connect(tcpsocket,SIGNAL(connected()),this,SLOT(TcpNewConnection()));
            FlagOpenClose=false;
            //qDebug() <<"Open: "<<Anet_typename.at(iNetTypeEnable);
            break;
        case NET_TYPE_TCPS:
            nui->pushButton_net_open->setText("监听中...");
            tcpserver=new QTcpServer(this);
            tcpserver->listen(QHostAddress::Any,nui->lineEdit_net_LocalPort->text().toInt());

            qDebug() <<"listen: "<<nui->lineEdit_net_LocalPort->text();
            connect(tcpserver,SIGNAL(newConnection()),this,SLOT(TcpNewConnection()));
            FlagOpenClose=false;
            break;
        default:
            break;
        }
        if(iNetTypeEnable==NET_TYPE_NULL){}
        else{
        }
    }else{
        Close();
        FlagOpenClose=true;
    }
}
void Ynet::TcpNewConnection(){


    switch (nui->comboBox_net_type->currentIndex()) {
    case NET_TYPE_UDP :
        break;
    case NET_TYPE_UDPB :
        break;
    case NET_TYPE_TCPS :
        tcpsocket=tcpserver->nextPendingConnection();
        // tcpsocket->
        //         connect(tcpsocket,SIGNAL(disconnected()),this,SLOT(Close()));
        connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(Recv()));
        iNetTypeEnable=nui->comboBox_net_type->currentIndex();
        nui->pushButton_net_open->setText("关闭TCPS");


        if(nui->checkBox_net_DestFollow->isChecked()==true){
            QHostAddress netAddrTemp;
            netAddrTemp.setAddress(tcpsocket->peerAddress().toIPv4Address());
            nui->lineEdit_net_DestIP->setText(netAddrTemp.toString());
            nui->lineEdit_net_DestPort->setText(QString::number(tcpsocket->peerPort()));
        }



        break;
    case NET_TYPE_TCPC :
        iNetTypeEnable=nui->comboBox_net_type->currentIndex();
        connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(Recv()));
        nui->pushButton_net_open->setText("关闭TCPC");
        break;
    default:
        break;
    }






}
void Ynet::Recv(){
    QByteArray requestData;
    QByteArray requestDataNet;
    QHostAddress netAddrTemp;
    quint16 netPortTemp;

    requestDataNet.clear();
    if(iNetTypeEnable!=NET_TYPE_NULL){

        int index;
        index=nui->comboBox_net_type->currentIndex();
        switch (index) {
        case NET_TYPE_TCPS://
        case NET_TYPE_TCPC://

            requestDataNet= tcpsocket->readAll();
            if((netAddrNow.toIPv4Address()!=tcpsocket->peerAddress().toIPv4Address())||(netPortNow!=tcpsocket->peerPort())){
                //netAddrNow=tcpsocket->peerAddress();
                netAddrNow.setAddress(tcpsocket->peerAddress().toIPv4Address());
                netPortNow=tcpsocket->peerPort();
                nui->textBrowser_net_recv->setTextColor(COLOR_RED);//设置字体颜色
                char buf[100]={0};
                sprintf(buf,"\n[%s:%d]\n",netAddrNow.toString().toUtf8().data(),netPortNow);
                //nui->textBrowser_net_recv->textCursor().insertText(QString(requestData));//add
                //sprintf()
                qDebug()<<"调试：：："<<buf;
                nui->textBrowser_net_recv->insertPlainText(QString("%1").arg(buf));
            }
            if(nui->checkBox_net_DestFollow->isChecked()==true){
                nui->lineEdit_net_DestIP->setText(netAddrNow.toString());
                nui->lineEdit_net_DestPort->setText(QString::number(netPortNow));
            }
            break;
        case NET_TYPE_UDP://
        case NET_TYPE_UDPB://


            while(udpsocket->hasPendingDatagrams()){
                requestDataNet.resize(udpsocket->pendingDatagramSize());
                udpsocket->readDatagram(requestDataNet.data(),requestDataNet.size(),&netAddrTemp,&netPortTemp);
            }
            if((netAddrNow.toIPv4Address()!=netAddrTemp.toIPv4Address())||(netPortNow!=netPortTemp))
            {
                netAddrNow.setAddress(netAddrTemp.toIPv4Address());
                netPortNow=netPortTemp;
                nui->textBrowser_net_recv->setTextColor(COLOR_RED);//设置字体颜色
                char buf[100]={0};
                sprintf(buf,"\n[%s:%d]\n",netAddrNow.toString().toUtf8().data(),netPortNow);
                //nui->textBrowser_net_recv->textCursor().insertText(QString(requestData));//add
                //sprintf()
                qDebug()<<"调试：：："<<buf;
                nui->textBrowser_net_recv->insertPlainText(QString("%1").arg(buf));
            }
            if(nui->checkBox_net_DestFollow->isChecked()==true){
                nui->lineEdit_net_DestIP->setText(netAddrNow.toString());
                nui->lineEdit_net_DestPort->setText(QString::number(netPortNow));
            }


            break;
        default:
            break;
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
            nui->textBrowser_net_recv->setTextColor(COLOR_BLACK);//设置字体颜色：正文黑色
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

void Ynet::RecvClear(){
    nui->textBrowser_net_recv->clear();
}
