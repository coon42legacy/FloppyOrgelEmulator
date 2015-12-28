#include "../common/config.h"
#include <QPainter>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {

  for(int y = 0; y < DISPLAY_RESOLUTION_Y; y++)
    for(int x = 0; x < DISPLAY_RESOLUTION_X; x++)
      setPixel(x, y, 128, 64, 64);
}

void MainWindow::setPixel(int x, int y, unsigned char red, unsigned char green, unsigned char blue) {
  image_[x][y].red   = red;
  image_[x][y].green = green;
  image_[x][y].blue  = blue;
}

void MainWindow::paintEvent(QPaintEvent *e) {
  Q_UNUSED(e);

  QPainter qp(this);
  drawPixels(&qp);
}

void MainWindow::drawPixels(QPainter *qp) {
  QPen pen(Qt::black, 1, Qt::SolidLine);
  qp->setPen(pen);

  for(int y = 0; y < DISPLAY_RESOLUTION_Y; y++)
    for(int x = 0; x < DISPLAY_RESOLUTION_X; x++) {
      pen.setColor(QColor(image_[x][y].red, image_[x][y].green, image_[x][y].blue));
      qp->setPen(pen);
      qp->drawLine(x, y, x, y);
    }
}



MainWindow::~MainWindow() {

}
