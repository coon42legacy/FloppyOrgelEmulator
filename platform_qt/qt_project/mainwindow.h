#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../../common/config.h"
#include <QMainWindow>

struct Pixel {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
};

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue);

protected:
    void paintEvent(QPaintEvent *event);
    void drawPixels(QPainter *qp);

private:
    Pixel image_[DISPLAY_RESOLUTION_X][DISPLAY_RESOLUTION_Y];
};

#endif // MAINWINDOW_H
