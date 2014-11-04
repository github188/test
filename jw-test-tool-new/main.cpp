#include "widget.h"
#include <QApplication>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
