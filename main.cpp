#include "PaintApplication.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PaintApplication w;
    w.setWindowTitle("Welcome To Paint Application");
    w.resize(1024, 800);
    w.show();
    return a.exec();
}
