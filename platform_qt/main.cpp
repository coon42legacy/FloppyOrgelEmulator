#include "mainwindow.h"
#include "../common/config.h"
#include <QApplication>

extern "C" {
  #include "../common/common_main.h"
}

#include "main.h"

void Thread::run() {
  common_main();
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    Thread t;
    w.setFixedWidth(DISPLAY_RESOLUTION_X);
    w.setFixedHeight(DISPLAY_RESOLUTION_Y);
    w.show();

    t.start();

    return a.exec();
}
