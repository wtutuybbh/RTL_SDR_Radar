#include "gui/MainWindow.h"
#include "AppCore/Core.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Core core;
    core.init();
    MainWindow m(&core);
    m.show();
    return a.exec();
}
