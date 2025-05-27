#include "mainwindow.h"
#include "gallery.h"
#include "splashscreen.h"
#include "view.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    SplashScreen splash;
    MainWindow mainwin;
    Gallery gallery;
    View view;

    splash.show();

    QObject::connect(&splash, &SplashScreen::startClicked, [&]() {
        splash.close();
        mainwin.show();
    });

    QObject::connect(&mainwin, &MainWindow::startClicked, [&]() {
        mainwin.close();
        gallery.show();
    });
    QObject::connect(&mainwin, &MainWindow::StartClicked, [&]() {
        mainwin.close();
        view.show();
    });

    QObject::connect(&gallery, &Gallery::startClicked, [&]() {
        gallery.close();
        mainwin.show();
    });

    return a.exec();
}
