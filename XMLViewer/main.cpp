#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication my_application(argc, argv);
    MainWindow window;
    window.show();
    return my_application.exec();
}
