#include "calculator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Calculator w;
    w.show();
     a.setWindowIcon(QIcon(":/AppIcon/assets/CalculatorIcon.png"));
    return a.exec();
}
