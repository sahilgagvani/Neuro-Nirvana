#ifndef CENTRALPROCESSOR_H
#define CENTRALPROCESSOR_H

#include "electrode.h"
#include <QObject>
#include <list>

class centralprocessor:public QObject{
    Q_OBJECT
public:
    centralprocessor(QObject *parent = nullptr);
    void saveData();

private:
    std::list<electrode> electrodes;
    int treatements;
};

#endif // CENTRALPROCESSOR_H
