#include "widget.h"
#include "ynet.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.show();
 //   ynet net=new net(w);
    return a.exec();
}
