#include "LabDemo.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LabDemo w;
    w.show();
    return a.exec();
}
