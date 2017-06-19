#ifndef YBASE_H
#define YBASE_H
#include <QWidget>
#include "stdio.h"
#include <iostream>
#include "ui_widget.h"
#include <QMessageBox>
#include <QInputDialog>
#define COLOR_BLACK QColor(0,0,0)

#define COLOR_GREEN QColor(0,255,0)
#define COLOR_RED QColor(255,0,0)
#define COLOR_BLUE QColor(0,0,255)

#define COLOR_YELLOW QColor(255,255,0)
#define COLOR_MAGENTA QColor(255,0,255)
#define COLOR_CYAN QColor(0,255,255)

namespace Ui {
class Ybase;
}
class Ybase: public QWidget
{
    Q_OBJECT
public:
     Ui::Widget *bui;
    explicit Ybase(QWidget *parent = 0);
    void Init();
    static QByteArray ByteUtf8toGb2312(QByteArray ByteUtf8);
    static QByteArray ByteGb2312toUtf8(QByteArray ByteGb2312);
    static QByteArray HexByteArrayToByteArray(QByteArray hexByteArray);
    static QByteArray ByteArrayToHexByteArray(QByteArray byteArray);
public slots:
    void SlotCalc();
};

#endif // YBASE_H
