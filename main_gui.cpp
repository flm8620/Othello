#include <QApplication>

#include "gamewindow.h"
#include <string>
#ifndef srcPath
#define srcPath "."
#endif

std::string aiFile = srcPath "/aiSave4.txt";

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qRegisterMetaType<::Color>("::Color");
    GameWindow gameWindow(aiFile,8);
    gameWindow.show();
    return app.exec();
}
