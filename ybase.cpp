
#include "ybase.h"
#include "ynet.h"
#include "yser.h"
#include <QWidget>
#include "widget.h"
#include "stdio.h"
#include <iostream>
#include "ui_widget.h"
#include <QMessageBox>
#include <QInputDialog>
#include "qprocess.h"

Ybase::Ybase(QWidget *parent) : QWidget(parent)
{
}

QByteArray Ybase::ByteUtf8toGb2312(QByteArray ByteUtf8){
    QTextCodec* utf8Codec= QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");
    QString strUnicode= utf8Codec->toUnicode(ByteUtf8);
    QByteArray ByteGb2312= gb2312Codec->fromUnicode(strUnicode);
    return ByteGb2312;
}
QByteArray Ybase::ByteGb2312toUtf8(QByteArray ByteGb2312){
    QTextCodec* utf8Codec= QTextCodec::codecForName("utf-8");
    QTextCodec* gb2312Codec = QTextCodec::codecForName("gb2312");
    QString strUnicode= gb2312Codec->toUnicode(ByteGb2312);
    QByteArray ByteUtf8= utf8Codec->fromUnicode(strUnicode);
    return ByteUtf8;
}
//16进制ASCII字符串转对应的二进制BUFFER

QByteArray Ybase::HexByteArrayToByteArray(QByteArray hexByteArray)
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
QByteArray Ybase::ByteArrayToHexByteArray(QByteArray byteArray)
{
    int i=0;
    char hexchar[(byteArray.length())*2+1]={0};
    char *charbytearrey=byteArray.data();
    for(i=0;i<byteArray.length();i++){
        sprintf(hexchar,"%s %02X",hexchar,(unsigned char)*(charbytearrey+i));
    }
    return QByteArray(hexchar);
}
void Ybase::SlotCalc()
{
    QProcess::startDetached("calc\n");
}
void Ybase::Init(){
   connect(bui->pushButton_calc,SIGNAL(clicked(bool)),this,SLOT(SlotCalc()));
}
