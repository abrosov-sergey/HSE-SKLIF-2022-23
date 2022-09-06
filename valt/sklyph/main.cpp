
#include <QApplication>

#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QString filePath;
    if (argc > 1) {
        filePath = argv[1];
    }

    sklyph::valt::MainWindow mainWindow(filePath);

    // Show main window and move it to center
//    QTimer::singleShot(1, &mainWindow, SLOT(showFullScreen()));
//
    mainWindow.show();
    mainWindow.moveToDesktopCenter();
//    mainWindow.setWindowState(Qt::WindowFullScreen);

    return app.exec();
}
