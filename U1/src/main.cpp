#include "mainform.h"
#include "./ui_mainform.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Set up the application icon
    a.setWindowIcon(QIcon(":/icons/pointpol.png"));
    MainForm w;
    w.show();
    return a.exec();
}
