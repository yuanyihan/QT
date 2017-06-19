
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
Yser::Yser(QWidget *parent) : QWidget(parent)
{

}

void Yser::Init()
{
    int i=0;
    bSerEnable=false;
    bSerTimerSentEnable=false;
    Aser_baud.clear();
    Aser_dataBits.clear();
    Aser_databitsname.clear();
    Aser_parity.clear();
    Aser_parityname.clear();
    Aser_stop.clear();
    Aser_stopname.clear();

    Aser_baud<<115200<<57600<<38400<<19200<<9600<<4800<<2400<<1200;

    Aser_databitsname<<"8"<<"7"<<"6"<<"5";
    Aser_dataBits<<QSerialPort::Data8<<QSerialPort::Data7<<QSerialPort::Data6<<QSerialPort::Data5;

    Aser_stopname<<"1"<<"1.5"<<"2";
    Aser_stop<<QSerialPort::OneStop<<QSerialPort::OneAndHalfStop<<QSerialPort::TwoStop;

    Aser_parityname<<"None"<<"Even"<<"Odd"<<"Space"<<"Mark";
    Aser_parity<<QSerialPort::NoParity<<QSerialPort::EvenParity<<QSerialPort::OddParity<<QSerialPort::SpaceParity<<QSerialPort::MarkParity;

    for(i=0;i<8;i++){
        sui->comboBox_ser_baud->addItem(QString::number(Aser_baud[i],10),i);
    }
    sui->comboBox_ser_baud->setCurrentIndex(0);
    for(i=0;i<4;i++){
        sui->comboBox_ser_data->addItem(Aser_databitsname[i],i);
    }
    sui->comboBox_ser_data->setCurrentIndex(0);

    for(i=0;i<3;i++){
        sui->comboBox_ser_stop->addItem(Aser_stopname[i],i);
    }
    sui->comboBox_ser_stop->setCurrentIndex(0);
    for(i=0;i<5;i++){
        sui->comboBox_ser_check->addItem(Aser_parityname[i],i);
    }
    sui->comboBox_ser_check->setCurrentIndex(0);

    QButtonGroup_ser_recv_AsciiHex=new QButtonGroup(this);
    QButtonGroup_ser_recv_AsciiHex->setExclusive(true);
    QButtonGroup_ser_recv_AsciiHex->addButton(sui->radioButton_ser_recv_ascii);
    QButtonGroup_ser_recv_AsciiHex->addButton(sui->radioButton_ser_recv_hex);
    sui->radioButton_ser_recv_ascii->setChecked(true);

    QButtonGroup_ser_sent_AsciiHex=new QButtonGroup(this);
    QButtonGroup_ser_sent_AsciiHex->setExclusive(true);
    QButtonGroup_ser_sent_AsciiHex->addButton(sui->radioButton_ser_sent_ascii);
    QButtonGroup_ser_sent_AsciiHex->addButton(sui->radioButton_ser_sent_hex);
    sui->radioButton_ser_sent_ascii->setChecked(true);

    QButtonGroup_ser_recv_Utf8Gb23=new QButtonGroup(this);
    QButtonGroup_ser_recv_Utf8Gb23->setExclusive(true);
    QButtonGroup_ser_recv_Utf8Gb23->addButton(sui->radioButton_ser_recv_gb2312);
    QButtonGroup_ser_recv_Utf8Gb23->addButton(sui->radioButton_ser_recv_utf8);
    sui->radioButton_ser_recv_gb2312->setChecked(true);

    QButtonGroup_ser_sent_Utf8Gb23=new QButtonGroup(this);
    QButtonGroup_ser_sent_Utf8Gb23->setExclusive(true);
    QButtonGroup_ser_sent_Utf8Gb23->addButton(sui->radioButton_ser_sent_gb2312);
    QButtonGroup_ser_sent_Utf8Gb23->addButton(sui->radioButton_ser_sent_utf8);
    sui->radioButton_ser_sent_gb2312->setChecked(true);

    sui->pushButton_SerTimeSent->setText(tr("打开定时发送"));
    sui->lineEdit_SerTimeSent->setText("1000");
    sui->textEdit_ser_sent->textCursor().insertText("YYH 袁一涵");
Scan();
connect(sui->pushButton_ser_open,SIGNAL(clicked(bool)),this,SLOT(Open()));
connect(sui->pushButton_ser_close,SIGNAL(clicked(bool)),this,SLOT(Close()));
}


void Yser::Scan(){

    int i=0;
    sui->comboBox_ser_com->clear();
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
            sui->comboBox_ser_com->addItem(info.portName(),i);
            serial.close();
        }
        i++;
    }
    sui->comboBox_ser_com->setCurrentIndex(0);
    sui->pushButton_ser_open->show();
    sui->pushButton_ser_scan->show();
    sui->pushButton_ser_close->hide();
    bSerEnable=false;

}
void Yser::Open(){
    my_serialport= new QSerialPort();
    qDebug()<<sui->comboBox_ser_com->currentText();
    my_serialport->setPortName(sui->comboBox_ser_com->currentText());
    my_serialport->open(QIODevice::ReadWrite);

    qDebug()<<sui->comboBox_ser_baud->currentText().toInt();
    my_serialport->setBaudRate(sui->comboBox_ser_baud->currentText().toInt());

    qDebug()<<sui->comboBox_ser_data->currentIndex();
    qDebug()<<Aser_databitsname.at(sui->comboBox_ser_data->currentIndex());
    my_serialport->setDataBits(Aser_dataBits.at(sui->comboBox_ser_data->currentIndex()));

    qDebug()<<sui->comboBox_ser_check->currentIndex();
    qDebug()<<Aser_parity.at(sui->comboBox_ser_check->currentIndex());
    my_serialport->setParity(Aser_parity.at(sui->comboBox_ser_check->currentIndex()));

    qDebug()<<sui->comboBox_ser_stop->currentIndex();
    qDebug()<<Aser_stopname.at(sui->comboBox_ser_stop->currentIndex());
    my_serialport->setStopBits(Aser_stop.at(sui->comboBox_ser_stop->currentIndex()));

    my_serialport->setFlowControl(QSerialPort::NoFlowControl);
    connect(my_serialport,SIGNAL(readyRead()),this,SLOT(Recv()));
    sui->pushButton_ser_open->hide();
    sui->pushButton_ser_scan->hide();
    sui->pushButton_ser_close->show();
    bSerEnable=true;

}
void Yser::Recv(){
    QByteArray requestData;
    if(bSerEnable==true){
        QByteArray requestDataserialport= my_serialport->readAll();
        if(requestDataserialport!= NULL)
        {
            if(sui->radioButton_ser_recv_hex->isChecked()==true){
                requestData=Ybase::ByteArrayToHexByteArray(requestDataserialport);}
            if(sui->radioButton_ser_recv_ascii->isChecked()==true){
                requestData=requestDataserialport;
                if(sui->radioButton_ser_recv_utf8->isChecked()==true){
                    requestData=requestData;
                }
                if(sui->radioButton_ser_recv_gb2312->isChecked()==true){
                    requestData=  Ybase::ByteGb2312toUtf8(requestData);
                }
            }

            if(sui->checkBox_ser_recv_newline->isChecked()==true){
                sui->textBrowser_ser_recv->append(QString(requestData));//new
            }
            else{
                sui->textBrowser_ser_recv->textCursor().insertText(QString(requestData));//add
            }
            sui->textBrowser_ser_recv->moveCursor(QTextCursor::End);
        }}
    requestData.clear();
}
void Yser::Close(){
    my_serialport->close();
    bSerEnable=false;
    Scan();
    sui->pushButton_ser_open->show();
    sui->pushButton_ser_scan->show();
    sui->pushButton_ser_close->hide();
    bSerEnable=true;
}

void Yser::Sent(){
    if(bSerEnable==true){
        QByteArray ByteSent;
        QByteArray Bytetextedit = sui->textEdit_ser_sent->toPlainText().toUtf8();

        if(sui->radioButton_ser_sent_ascii->isChecked()==true){
            if(sui->radioButton_ser_sent_utf8->isChecked()==true){
                ByteSent=(Bytetextedit);
            }
            if(sui->radioButton_ser_sent_gb2312->isChecked()==true){
                ByteSent=  Ybase::ByteUtf8toGb2312((Bytetextedit));
            }
        }
        if(sui->radioButton_ser_sent_hex->isChecked()==true){
            ByteSent=Ybase::HexByteArrayToByteArray(Bytetextedit);

        }

        if(sui->checkBox_ser_sent_newliner->isChecked()==true){
            ByteSent=ByteSent.append("\r");
        }
        if(sui->checkBox_ser_sent_newlinen->isChecked()==true){
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

void Yser::SlotTimerSet(){
    if(bSerTimerSentEnable==false){
        sui->pushButton_SerTimeSent->setText(tr("关闭定时发送"));
        serTimerSent = new QTimer(this);
        connect(serTimerSent, SIGNAL(timeout()), this, SLOT(Sent()));
        serTimerSent->start(sui->lineEdit_SerTimeSent->text().toInt());
        bSerTimerSentEnable=true;
    }else{
        sui->pushButton_SerTimeSent->setText(tr("打开定时发送"));
        serTimerSent->stop();
        bSerTimerSentEnable=false;
    }
}
void Yser::RecvClear(){
    sui->textBrowser_ser_recv->clear();
}
void Yser::SlotRecvAsciiHex(){
    if(sui->radioButton_ser_recv_ascii->isChecked()==true){
        sui->radioButton_ser_recv_gb2312->setEnabled(true);
        sui->radioButton_ser_recv_utf8->setEnabled(true);
    }
    if(sui->radioButton_ser_recv_hex->isChecked()==true){
        sui->radioButton_ser_recv_gb2312->setEnabled(false);
        sui->radioButton_ser_recv_utf8->setEnabled(false);
    }
}
void Yser::SlotSentAsciiHex(){
    QByteArray ByteTEXT = (sui->textEdit_ser_sent->toPlainText().toUtf8());
    QByteArray ByteEDIT=ByteTEXT;
    if(sui->radioButton_ser_sent_ascii->isChecked()==true){

        //char* pchar
        if(sui->radioButton_ser_sent_utf8->isChecked()==true){
            ByteEDIT= (Ybase::HexByteArrayToByteArray(ByteTEXT));
        }
        if(sui->radioButton_ser_sent_gb2312->isChecked()==true){
            ByteEDIT=  Ybase::ByteGb2312toUtf8(Ybase::HexByteArrayToByteArray(ByteTEXT));
        }


        sui->radioButton_ser_sent_gb2312->setEnabled(true);
        sui->radioButton_ser_sent_utf8->setEnabled(true);
    }
    if(sui->radioButton_ser_sent_hex->isChecked()==true){


        if(sui->radioButton_ser_sent_utf8->isChecked()==true){
            ByteEDIT= Ybase::ByteArrayToHexByteArray(ByteTEXT);
        }
        if(sui->radioButton_ser_sent_gb2312->isChecked()==true){
            ByteEDIT= Ybase::ByteArrayToHexByteArray(Ybase::ByteUtf8toGb2312(ByteTEXT));
        }


        sui->radioButton_ser_sent_gb2312->setEnabled(false);
        sui->radioButton_ser_sent_utf8->setEnabled(false);
    }
    sui->textEdit_ser_sent->clear();
    sui->textEdit_ser_sent->textCursor().insertText(QString(ByteEDIT));
}

void Yser::SlotSentUtf8Gb2312(){

}

