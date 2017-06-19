#include "widget.h"
#include "stdio.h"
#include <iostream>
#include "ui_widget.h"
#include <QMessageBox>
#include <QInputDialog>
#include "qprocess.h"
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    DataInit();
}

Widget::~Widget()
{
    delete ui;
}
Widget::DataInit(){
    int i=0;
    //serial
    bSerEnable=false;
    bSerTimerSentEnable=false;
    bNetTimerSentEnable=false;
    Aser_baud.clear();
    Aser_dataBits.clear();
    Aser_databitsname.clear();
    Aser_parity.clear();
    Aser_parityname.clear();
    Aser_stop.clear();
    Aser_stopname.clear();

    Anet_typename.clear();
    Anet_type.clear();

    Aser_baud<<115200<<57600<<38400<<19200<<9600<<4800<<2400<<1200;

    Aser_databitsname<<"8"<<"7"<<"6"<<"5";
    Aser_dataBits<<QSerialPort::Data8<<QSerialPort::Data7<<QSerialPort::Data6<<QSerialPort::Data5;

    Aser_stopname<<"1"<<"1.5"<<"2";
    Aser_stop<<QSerialPort::OneStop<<QSerialPort::OneAndHalfStop<<QSerialPort::TwoStop;

    Aser_parityname<<"None"<<"Even"<<"Odd"<<"Space"<<"Mark";
    Aser_parity<<QSerialPort::NoParity<<QSerialPort::EvenParity<<QSerialPort::OddParity<<QSerialPort::SpaceParity<<QSerialPort::MarkParity;

    Anet_typename<<"TCP_Server"<<"TCP_Client"<<"UDP"<<"UDP_Broadcast";
    Anet_type<<NET_TYPE_TCPS<<NET_TYPE_TCPC<<NET_TYPE_UDP<<NET_TYPE_UDPB;
    for(i=0;i<8;i++){
        ui->comboBox_ser_baud->addItem(QString::number(Aser_baud[i],10),i);
    }
    ui->comboBox_ser_baud->setCurrentIndex(0);
    for(i=0;i<4;i++){
        ui->comboBox_ser_data->addItem(Aser_databitsname[i],i);
    }
    ui->comboBox_ser_data->setCurrentIndex(0);

    for(i=0;i<3;i++){
        ui->comboBox_ser_stop->addItem(Aser_stopname[i],i);
    }
    ui->comboBox_ser_stop->setCurrentIndex(0);
    for(i=0;i<5;i++){
        ui->comboBox_ser_check->addItem(Aser_parityname[i],i);
    }
    ui->comboBox_ser_check->setCurrentIndex(0);
    for(i=0;i<4;i++){
        ui->comboBox_net_type->addItem(Anet_typename[i],i);
    }
    ui->comboBox_net_type->setCurrentIndex(2);//UDP
    QButtonGroup_ser_recv_AsciiHex=new QButtonGroup(this);
    QButtonGroup_ser_recv_AsciiHex->setExclusive(true);
    QButtonGroup_ser_recv_AsciiHex->addButton(ui->radioButton_ser_recv_ascii);
    QButtonGroup_ser_recv_AsciiHex->addButton(ui->radioButton_ser_recv_hex);
    ui->radioButton_ser_recv_ascii->setChecked(true);

    QButtonGroup_ser_sent_AsciiHex=new QButtonGroup(this);
    QButtonGroup_ser_sent_AsciiHex->setExclusive(true);
    QButtonGroup_ser_sent_AsciiHex->addButton(ui->radioButton_ser_sent_ascii);
    QButtonGroup_ser_sent_AsciiHex->addButton(ui->radioButton_ser_sent_hex);
    ui->radioButton_ser_sent_ascii->setChecked(true);

    QButtonGroup_ser_recv_Utf8Gb23=new QButtonGroup(this);
    QButtonGroup_ser_recv_Utf8Gb23->setExclusive(true);
    QButtonGroup_ser_recv_Utf8Gb23->addButton(ui->radioButton_ser_recv_gb2312);
    QButtonGroup_ser_recv_Utf8Gb23->addButton(ui->radioButton_ser_recv_utf8);
    ui->radioButton_ser_recv_gb2312->setChecked(true);

    QButtonGroup_ser_sent_Utf8Gb23=new QButtonGroup(this);
    QButtonGroup_ser_sent_Utf8Gb23->setExclusive(true);
    QButtonGroup_ser_sent_Utf8Gb23->addButton(ui->radioButton_ser_sent_gb2312);
    QButtonGroup_ser_sent_Utf8Gb23->addButton(ui->radioButton_ser_sent_utf8);
    ui->radioButton_ser_sent_gb2312->setChecked(true);

    QButtonGroup_net_recv_AsciiHex=new QButtonGroup(this);
    QButtonGroup_net_recv_AsciiHex->setExclusive(true);
    QButtonGroup_net_recv_AsciiHex->addButton(ui->radioButton_net_recv_ascii);
    QButtonGroup_net_recv_AsciiHex->addButton(ui->radioButton_net_recv_hex);
    ui->radioButton_net_recv_ascii->setChecked(true);

    QButtonGroup_net_sent_AsciiHex=new QButtonGroup(this);
    QButtonGroup_net_sent_AsciiHex->setExclusive(true);
    QButtonGroup_net_sent_AsciiHex->addButton(ui->radioButton_net_sent_ascii);
    QButtonGroup_net_sent_AsciiHex->addButton(ui->radioButton_net_sent_hex);
    ui->radioButton_net_sent_ascii->setChecked(true);

    QButtonGroup_net_recv_Utf8Gb23=new QButtonGroup(this);
    QButtonGroup_net_recv_Utf8Gb23->setExclusive(true);
    QButtonGroup_net_recv_Utf8Gb23->addButton(ui->radioButton_net_recv_gb2312);
    QButtonGroup_net_recv_Utf8Gb23->addButton(ui->radioButton_net_recv_utf8);
    ui->radioButton_net_recv_gb2312->setChecked(true);

    QButtonGroup_net_sent_Utf8Gb23=new QButtonGroup(this);
    QButtonGroup_net_sent_Utf8Gb23->setExclusive(true);
    QButtonGroup_net_sent_Utf8Gb23->addButton(ui->radioButton_net_sent_gb2312);
    QButtonGroup_net_sent_Utf8Gb23->addButton(ui->radioButton_net_sent_utf8);
    ui->radioButton_net_sent_gb2312->setChecked(true);

    QButtonGroup_net_anyport=new QButtonGroup(this);
    QButtonGroup_net_anyport->setExclusive(true);
    QButtonGroup_net_anyport->addButton(ui->radioButton_net_localportany);
    QButtonGroup_net_anyport->addButton(ui->radioButton_net_localportblind);
    ui->radioButton_net_localportblind->setChecked(true);

    ui->pushButton_SerTimeSent->setText(tr("打开定时发送"));
    ui->lineEdit_SerTimeSent->setText("1000");
    ui->textEdit_ser_sent->textCursor().insertText("YYH 袁一涵");

    ui->pushButton_NetTimeSent->setText(tr("打开定时发送"));
    ui->lineEdit_NetTimeSent->setText("1000");
    ui->textEdit_net_sent->textCursor().insertText("YYH 袁一涵");

    ui->lineEdit_net_DestIP->setText("127.0.0.1");
    ui->lineEdit_net_DestPort->setText("61000");
    ui->lineEdit_net_LocalPort->setText("62000");
    on_pushButton_ser_scan_clicked();
    on_pushButton_net_scan_clicked();
    connect(QButtonGroup_net_anyport,SIGNAL(buttonClicked(int)),this,SLOT(buttonGroup_netAnyPort(int)));
    return 1;
}
void Widget::buttonGroup_netAnyPort(int id){
    if(ui->radioButton_net_localportany->isChecked()==true){
        ui->lineEdit_net_LocalPort->setEnabled(false);
    }
    if(ui->radioButton_net_localportblind->isChecked()==true){
        ui->lineEdit_net_LocalPort->setEnabled(true);
    }
}
void Widget::on_pushButton_ser_open_clicked()
{
    my_serialport= new QSerialPort();
    qDebug()<<ui->comboBox_ser_com->currentText();
    my_serialport->setPortName(ui->comboBox_ser_com->currentText());
    my_serialport->open(QIODevice::ReadWrite);

    qDebug()<<ui->comboBox_ser_baud->currentText().toInt();
    my_serialport->setBaudRate(ui->comboBox_ser_baud->currentText().toInt());

    qDebug()<<ui->comboBox_ser_data->currentIndex();
    qDebug()<<Aser_databitsname.at(ui->comboBox_ser_data->currentIndex());
    my_serialport->setDataBits(Aser_dataBits.at(ui->comboBox_ser_data->currentIndex()));

    qDebug()<<ui->comboBox_ser_check->currentIndex();
    qDebug()<<Aser_parity.at(ui->comboBox_ser_check->currentIndex());
    my_serialport->setParity(Aser_parity.at(ui->comboBox_ser_check->currentIndex()));

    qDebug()<<ui->comboBox_ser_stop->currentIndex();
    qDebug()<<Aser_stopname.at(ui->comboBox_ser_stop->currentIndex());
    my_serialport->setStopBits(Aser_stop.at(ui->comboBox_ser_stop->currentIndex()));

    my_serialport->setFlowControl(QSerialPort::NoFlowControl);
    connect(my_serialport,SIGNAL(readyRead()),this,SLOT(SerRecv()));
    ui->pushButton_ser_open->hide();
    ui->pushButton_ser_scan->hide();
    ui->pushButton_ser_close->show();
    bSerEnable=true;
}
void Widget::SerRecv()
{
    QByteArray requestData;
    if(bSerEnable==true){
        QByteArray requestDataserialport= my_serialport->readAll();
        if(requestDataserialport!= NULL)
        {
            if(ui->radioButton_ser_recv_hex->isChecked()==true){
                requestData=ByteArrayToHexByteArray(requestDataserialport);}
            if(ui->radioButton_ser_recv_ascii->isChecked()==true){
                requestData=requestDataserialport;
                if(ui->radioButton_ser_recv_utf8->isChecked()==true){
                    requestData=requestData;
                }
                if(ui->radioButton_ser_recv_gb2312->isChecked()==true){
                    requestData=  ByteGb2312toUtf8(requestData);
                }
            }

            if(ui->checkBox_ser_recv_newline->isChecked()==true){
                ui->textBrowser_ser_recv->append(QString(requestData));//new
            }
            else{
                ui->textBrowser_ser_recv->textCursor().insertText(QString(requestData));//add
            }

            ui->textBrowser_ser_recv->moveCursor(QTextCursor::End);

        }}
    requestData.clear();
}
void Widget::on_pushButton_ser_sent_clicked()
{
    fSerSent();

}

void Widget::on_pushButton_ser_close_clicked()
{
    my_serialport->close();
    bSerEnable=false;
    on_pushButton_ser_scan_clicked();
}


void Widget::on_pushButton_ser_scan_clicked()
{
    int i=0;
    ui->comboBox_ser_com->clear();
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
    {
        qDebug() << "Name        : " << info.portName();
        qDebug() << "Description : " << info.description();
        qDebug() << "Manufacturer: " << info.manufacturer();

        // Example use QSerialPort
        QSerialPort serial;
        serial.setPort(info);

        if (serial.open(QIODevice::ReadWrite))
        {
            ui->comboBox_ser_com->addItem(info.portName(),i);
            serial.close();
        }
        i++;
    }
    ui->comboBox_ser_com->setCurrentIndex(0);
    ui->pushButton_ser_open->show();
    ui->pushButton_ser_scan->show();
    ui->pushButton_ser_close->hide();
    bSerEnable=false;
}

QByteArray Widget::ByteUtf8toGb2312(QByteArray ByteUtf8){
    QTextCodec* utf8Codec= QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");
    QString strUnicode= utf8Codec->toUnicode(ByteUtf8);
    QByteArray ByteGb2312= gb2312Codec->fromUnicode(strUnicode);
    return ByteGb2312;
}
QByteArray Widget::ByteGb2312toUtf8(QByteArray ByteGb2312){
    QTextCodec* utf8Codec= QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");
    QString strUnicode= gb2312Codec->toUnicode(ByteGb2312);
    QByteArray ByteUtf8= utf8Codec->fromUnicode(strUnicode);
    return ByteUtf8;
}
//16进制ASCII字符串转对应的二进制BUFFER

QByteArray Widget::HexByteArrayToByteArray(QByteArray hexByteArray)
{
    char * pHexChar;
    int i=0,j=0,hex_len=0;
    char hexchar[hexByteArray.length()+1]={0};
    pHexChar=(char *)hexByteArray.toUpper().data();
    for(i=0;i<hexByteArray.length();i++){
        if((*(pHexChar+i)>='0')&&(*(pHexChar+i)<='9'))
        {
            hexchar[j]=*(pHexChar+i);
            j++;
        }
        else if((*(pHexChar+i)>='A')&&(*(pHexChar+i)<='F'))
        {
            hexchar[j]=*(pHexChar+i);
            j++;
        }
    }
    if(j%2==1){
        hexchar[j]='0';
        j++;
    }

    hex_len=j;
    char charbyte[hex_len/2+1]={0};
    for(i=0;i<hex_len;i=i+2){
        if((hexchar[i]>='0')&&(hexchar[i]<='9'))
        {
            hexchar[i]=hexchar[i]-'0';
        }
        else if((hexchar[i]>='A')&&(hexchar[i]<='F'))
        {
            hexchar[i]=hexchar[i]-'A'+10;
        }
        if((hexchar[i+1]>='0')&&(hexchar[i+1]<='9'))
        {
            hexchar[i+1]=hexchar[i+1]-'0';
        }
        else if((hexchar[i+1]>='A')&&(hexchar[i+1]<='F'))
        {
            hexchar[i+1]=hexchar[i+1]-'A'+10;
        }
        charbyte[i/2]=(char)hexchar[i]*16+hexchar[i+1];
    }
    return QByteArray((char *)charbyte);
}
//ASCII字符串转十六制字符串
QByteArray Widget::ByteArrayToHexByteArray(QByteArray byteArray)
{
    int i=0;
    char hexchar[(byteArray.length())*2+1]={0};
    char *charbytearrey=byteArray.data();
    for(i=0;i<byteArray.length();i++){
        sprintf(hexchar,"%s %02X",hexchar,(unsigned char)*(charbytearrey+i));
    }
    return QByteArray(hexchar);
}

void Widget::on_radioButton_ser_sent_ascii_clicked()
{
    QByteArray ByteEDIT;
    QByteArray ByteHEX = (ui->textEdit_ser_sent->toPlainText().toLatin1());
    //char* pchar
    if(ui->radioButton_ser_sent_utf8->isChecked()==true){
        ByteEDIT= (HexByteArrayToByteArray(ByteHEX));
    }
    if(ui->radioButton_ser_sent_gb2312->isChecked()==true){
        ByteEDIT=  ByteGb2312toUtf8(HexByteArrayToByteArray(ByteHEX));
    }
    ui->textEdit_ser_sent->clear();
    ui->textEdit_ser_sent->textCursor().insertText(QString(ByteEDIT));
    ui->radioButton_ser_sent_gb2312->setEnabled(true);
    ui->radioButton_ser_sent_utf8->setEnabled(true);
}

void Widget::on_radioButton_ser_sent_hex_clicked()
{
    QByteArray ByteEDIT;
    QByteArray ByteUTF8 = ui->textEdit_ser_sent->toPlainText().toUtf8();
    if(ui->radioButton_ser_sent_utf8->isChecked()==true){
        ByteEDIT= ByteArrayToHexByteArray(ByteUTF8);
    }
    if(ui->radioButton_ser_sent_gb2312->isChecked()==true){
        ByteEDIT= ByteArrayToHexByteArray(ByteUtf8toGb2312(ByteUTF8));
    }
    ui->textEdit_ser_sent->clear();
    ui->textEdit_ser_sent->textCursor().insertText(QString(ByteEDIT));
    ui->radioButton_ser_sent_gb2312->setEnabled(false);
    ui->radioButton_ser_sent_utf8->setEnabled(false);
}

void Widget::on_radioButton_ser_recv_ascii_clicked()
{
    ui->radioButton_ser_recv_gb2312->setEnabled(true);
    ui->radioButton_ser_recv_utf8->setEnabled(true);
}

void Widget::on_radioButton_ser_recv_hex_clicked()
{
    //QString
    ui->radioButton_ser_recv_gb2312->setEnabled(false);
    ui->radioButton_ser_recv_utf8->setEnabled(false);
}

void Widget::on_pushButton_ser_recv_clear_clicked()
{
    ui->textBrowser_ser_recv->clear();
}

void Widget::on_pushButton_calc_clicked()
{
    QProcess::startDetached("calc\n");
}

void Widget::on_pushButton_SerTimeSent_clicked()
{
    if(bSerTimerSentEnable==false){
        ui->pushButton_SerTimeSent->setText(tr("关闭定时发送"));
        serTimerSent = new QTimer(this);
        connect(serTimerSent, SIGNAL(timeout()), this, SLOT(fSerSent()));
        serTimerSent->start(ui->lineEdit_SerTimeSent->text().toInt());

        //
        bSerTimerSentEnable=true;
    }else{
        ui->pushButton_SerTimeSent->setText(tr("打开定时发送"));
        serTimerSent->stop();
        bSerTimerSentEnable=false;
    }

}
void Widget::fSerSent(){
    if(bSerEnable==true){
        QByteArray ByteSent;
        QByteArray Bytetextedit = ui->textEdit_ser_sent->toPlainText().toUtf8();

        if(ui->radioButton_ser_sent_ascii->isChecked()==true){
            if(ui->radioButton_ser_sent_utf8->isChecked()==true){
                ByteSent=(Bytetextedit);
            }
            if(ui->radioButton_ser_sent_gb2312->isChecked()==true){
                ByteSent=  ByteUtf8toGb2312((Bytetextedit));
            }
        }
        if(ui->radioButton_ser_sent_hex->isChecked()==true){
            ByteSent=HexByteArrayToByteArray(Bytetextedit);

        }

        if(ui->checkBox_ser_sent_newliner->isChecked()==true){
            ByteSent=ByteSent.append("\r");
        }
        if(ui->checkBox_ser_sent_newlinen->isChecked()==true){
            ByteSent=ByteSent.append("\n");
        }

        my_serialport->write( ByteSent);
        // my_serialport->write(ByteUtf8);
    }else{
        int rb
                = QMessageBox::warning(NULL,
                                       tr("警告"),
                                       tr("未打开串口！请先打开一个串口"),
                                       QMessageBox::Yes,
                                       QMessageBox::Yes);
        if(rb == QMessageBox::Yes)
        {
            qDebug()<<"警告!先打开一个串口";
        }

    }
}

void Widget::on_pushButton_net_scan_clicked()
{
    int i=0;
    QHostAddress HOSTaddress;
    QString localHostName = QHostInfo::localHostName();

    qDebug() <<"localHostName:"<<localHostName;
    QHostInfo info = QHostInfo::fromName(localHostName);
    ui->comboBox_net_localIP->clear();
    ui->comboBox_net_localIP->addItem("127.0.0.1",i);
    i++;
    foreach(HOSTaddress,info.addresses())
    {
        if(HOSTaddress.protocol() == QAbstractSocket::IPv4Protocol)
        {
            qDebug() <<"IPV4 Address: "<< HOSTaddress.toString();
            ui->comboBox_net_localIP->addItem(HOSTaddress.toString(),i);
            i++;
        }
    }
    ui->comboBox_net_localIP->setCurrentIndex(0);
    bNetEnable=false;
}




void Widget::on_comboBox_net_type_currentIndexChanged(int index)
{
    //Anet_type<<NET_TYPE_TCPS<<NET_TYPE_TCPC<<NET_TYPE_UDP<<NET_TYPE_UDPB;
    bNetEnable=false;
    ui->pushButton_net_open->setText("打开");
    switch (index) {
    case NET_TYPE_TCPS://
        ui->lineEdit_net_DestIP->setEnabled(false);
        ui->lineEdit_net_DestPort->setEnabled(false);
        break;
    case NET_TYPE_TCPC://
        ui->lineEdit_net_DestIP->setEnabled(true);
        ui->lineEdit_net_DestPort->setEnabled(true);

        break;
    case NET_TYPE_UDP://
        ui->lineEdit_net_DestIP->setEnabled(true);
        ui->lineEdit_net_DestPort->setEnabled(true);

        break;
    case NET_TYPE_UDPB://
        ui->lineEdit_net_DestIP->setEnabled(true);
        ui->lineEdit_net_DestPort->setEnabled(true);

        break;
    default:
        break;
    }
}

void Widget::on_pushButton_NetTimeSent_clicked()
{
    if(bNetTimerSentEnable==false){
        ui->pushButton_NetTimeSent->setText(tr("关闭定时发送"));
        netTimerSent = new QTimer(this);
        connect(netTimerSent, SIGNAL(timeout()), this, SLOT(fNetSent()));
        netTimerSent->start(ui->lineEdit_NetTimeSent->text().toInt());

        //
        bNetTimerSentEnable=true;
    }else{
        ui->pushButton_NetTimeSent->setText(tr("打开定时发送"));
        netTimerSent->stop();
        bNetTimerSentEnable=false;
    }
}


void Widget::on_pushButton_net_sent_clicked()
{
    fNetSent();
}
void Widget::fNetSent(){

    if(bNetEnable==true){
        QByteArray ByteSent;
        QByteArray Bytetextedit = ui->textEdit_net_sent->toPlainText().toUtf8();

        if(ui->radioButton_net_sent_ascii->isChecked()==true){
            if(ui->radioButton_net_sent_utf8->isChecked()==true){
                ByteSent=(Bytetextedit);
            }
            if(ui->radioButton_net_sent_gb2312->isChecked()==true){
                ByteSent=  ByteUtf8toGb2312((Bytetextedit));
            }
        }
        if(ui->radioButton_net_sent_hex->isChecked()==true){
            ByteSent=HexByteArrayToByteArray(Bytetextedit);

        }

        if(ui->checkBox_net_sent_newliner->isChecked()==true){
            ByteSent=ByteSent.append("\r");
        }
        if(ui->checkBox_net_sent_newlinen->isChecked()==true){
            ByteSent=ByteSent.append("\n");
        }

        QHostAddress sentAddress;

        switch (ui->comboBox_net_type->currentIndex()) {
        case NET_TYPE_UDP :
            //向特定IP发送
            sentAddress = QHostAddress(ui->lineEdit_net_DestIP->text());
            udpsocket->writeDatagram(ByteSent, ByteSent.length(),sentAddress,ui->lineEdit_net_DestPort->text().toInt());
            break;
        case NET_TYPE_UDPB :
            //UDP广播
            udpsocket->writeDatagram(ByteSent,ByteSent.length(),QHostAddress::Broadcast,ui->lineEdit_net_DestPort->text().toInt());
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
void Widget::on_pushButton_net_open_clicked()
{
    if(bNetEnable==false){
        switch (ui->comboBox_net_type->currentIndex()) {
        case NET_TYPE_UDP:
            bNetEnable=true;
            ui->pushButton_net_open->setText("关闭UDP");
            udpsocket=new QUdpSocket(this);
            if(ui->radioButton_net_localportblind->isChecked()==true){
                udpsocket->bind(ui->lineEdit_net_LocalPort->text().toInt());
            }
            connect(udpsocket,SIGNAL(readyRead()),this,SLOT(NetRecv()));
            break;
        case NET_TYPE_UDPB:
            bNetEnable=true;
            ui->pushButton_net_open->setText("关闭UDP_B");
            udpsocket=new QUdpSocket(this);
            if(ui->radioButton_net_localportblind->isChecked()==true){
                udpsocket->bind(ui->lineEdit_net_LocalPort->text().toInt());
            }
            connect(udpsocket,SIGNAL(readyRead()),this,SLOT(NetRecv()));
            break;
        default:
            break;
        }
    }else{
        bNetEnable=false;
        udpsocket->close();
        ui->pushButton_net_open->setText("打开");
    }
}


void Widget::on_pushButton_net_recv_clear_clicked()
{
    ui->textBrowser_net_recv->clear();
}

void Widget::on_radioButton_net_sent_ascii_clicked()
{
    QByteArray ByteEDIT;
    QByteArray ByteHEX = (ui->textEdit_net_sent->toPlainText().toLatin1());
    //char* pchar
    if(ui->radioButton_net_sent_utf8->isChecked()==true){
        ByteEDIT= (HexByteArrayToByteArray(ByteHEX));
    }
    if(ui->radioButton_net_sent_gb2312->isChecked()==true){
        ByteEDIT=  ByteGb2312toUtf8(HexByteArrayToByteArray(ByteHEX));
    }
    ui->textEdit_net_sent->clear();
    ui->textEdit_net_sent->textCursor().insertText(QString(ByteEDIT));
    ui->radioButton_net_sent_gb2312->setEnabled(true);
    ui->radioButton_net_sent_utf8->setEnabled(true);
}

void Widget::on_radioButton_net_sent_hex_clicked()
{
    QByteArray ByteEDIT;
    QByteArray ByteUTF8 = ui->textEdit_net_sent->toPlainText().toUtf8();
    if(ui->radioButton_net_sent_utf8->isChecked()==true){
        ByteEDIT= ByteArrayToHexByteArray(ByteUTF8);
    }
    if(ui->radioButton_net_sent_gb2312->isChecked()==true){
        ByteEDIT= ByteArrayToHexByteArray(ByteUtf8toGb2312(ByteUTF8));
    }
    ui->textEdit_net_sent->clear();
    ui->textEdit_net_sent->textCursor().insertText(QString(ByteEDIT));
    ui->radioButton_net_sent_gb2312->setEnabled(false);
    ui->radioButton_net_sent_utf8->setEnabled(false);
}

void Widget::on_radioButton_net_recv_ascii_clicked()
{
    ui->radioButton_net_recv_gb2312->setEnabled(true);
    ui->radioButton_net_recv_utf8->setEnabled(true);
}

void Widget::on_radioButton_net_recv_hex_clicked()
{
    ui->radioButton_net_recv_gb2312->setEnabled(false);
    ui->radioButton_net_recv_utf8->setEnabled(false);
}
void Widget::NetRecv()
{
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
            if(ui->radioButton_net_recv_hex->isChecked()==true){
                requestData=ByteArrayToHexByteArray(requestDataNet);}
            if(ui->radioButton_net_recv_ascii->isChecked()==true){
                requestData=requestDataNet;
                if(ui->radioButton_net_recv_utf8->isChecked()==true){
                    requestData=requestData;
                }
                if(ui->radioButton_net_recv_gb2312->isChecked()==true){
                    requestData=  ByteGb2312toUtf8(requestData);
                }
            }

            if(ui->checkBox_net_recv_newline->isChecked()==true){
                ui->textBrowser_net_recv->append(QString(requestData));//new
            }
            else{
                ui->textBrowser_net_recv->textCursor().insertText(QString(requestData));//add
            }

            ui->textBrowser_net_recv->moveCursor(QTextCursor::End);

        }}
    requestData.clear();
}
void Widget::fNetClose()
{
    ui->radioButton_net_recv_gb2312->setEnabled(false);
    ui->radioButton_net_recv_utf8->setEnabled(false);
}
