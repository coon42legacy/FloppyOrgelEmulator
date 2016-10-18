#ifndef MAIN_H
#define MAIN_H

#include <QThread>

class Thread : public QThread {
    Q_OBJECT

protected:
    void run();
};

#endif // MAIN_H
