#ifndef WIDGET_H
#define WIDGET_H
#include "ybase.h"
#include "ynet.h"
#include "yser.h"
#include <QWidget>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QtCore>
#include <QMessageBox>
#include <QInputDialog>
#include "time.h"
#include <QtNetwork>
#include <QButtonGroup>
#include <QWidget>
#include <widget.h>
#include "widget.h"
#include "stdio.h"
#include <iostream>
#include "ui_widget.h"
#include <QMessageBox>
#include <QInputDialog>
#include "qprocess.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
public:  
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private slots:
private:
    Ui::Widget *ui;
    Ybase *ybase;
    Ynet *ynet;
    Yser *yser;
};

#endif // WIDGET_H
