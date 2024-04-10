#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setFixedSize(APP_WINDOW_WIDTH, APP_WINDOW_HEIGHT);
    w.setWindowTitle("Plot");
    w.show();
    return a.exec();
}
