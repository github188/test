#include "widget.h"
#include <QApplication>
#include <string.h>

int noip = 0;
int main(int argc, char *argv[])
{
    if (argc >= 2) {
        if (strcmp (argv[1], "-noip") == 0)
            noip = 1;
    }
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}
