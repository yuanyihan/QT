#ifndef YNET_H
#define YNET_H

#include <QWidget>
#include <QWidget>
#include <widget.h>
#include "widget.h"
#include "stdio.h"
#include <iostream>
#include "ui_widget.h"
#include <QMessageBox>
#include <QInputDialog>
#include "qprocess.h"
class ynet : public QWidget
{
    Q_OBJECT
public:
    Ui::Widget *nui;
    explicit ynet(QWidget *parent = 0);

signals:

public slots:
};

#endif // YNET_H
