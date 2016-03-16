#include <QApplication>

#include "gamewindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qRegisterMetaType<::Color>("::Color");
    GameWindow gameWindow(8);
    gameWindow.show();
    return app.exec();
}
