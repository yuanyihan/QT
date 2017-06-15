#include "serial.h"
#include "ui_widget.h"

Serial::Serial(QWidget *parent) :
    Serial(parent),
    ui(new Ui::Serial)
{
    ui->setupUi(this);
}

Serial::~Serial()
{
    delete ui;
}
