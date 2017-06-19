#include "ynet.h"
#include <QWidget>
#include <widget.h>
#include "widget.h"
#include "stdio.h"
#include <iostream>
#include "ui_widget.h"
#include <QMessageBox>
#include <QInputDialog>
#include "qprocess.h"
ynet::ynet(QWidget *parent) : QWidget(parent)
{
nui=Widget::ui;
}
