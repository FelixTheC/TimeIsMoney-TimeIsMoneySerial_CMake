#include "serialoptions.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SerialOptions w;
    w.show();
    return a.exec();
}
