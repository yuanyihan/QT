#include "ybase.h"
#include "ynet.h"
#include "yser.h"
#include <QWidget>
#include <widget.h>
#include "widget.h"
#include "stdio.h"
#include <iostream>
#include "ui_widget.h"
#include <QMessageBox>
#include <QInputDialog>
#include "qprocess.h"
Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->tabWidget->setCurrentIndex(0);
    yser=new Yser();
    ynet=new Ynet();
    ybase=new Ybase();
    yser->sui = ui;
    ynet->nui=ui;
    ybase->bui=ui;
    yser->Init();
    ybase->Init();
    ynet->Init();
}
Widget::~Widget()
{
    delete ui;
}
