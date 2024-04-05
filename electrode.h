#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>

class electrode:public QObject{
    Q_OBJECT

public:
    electrode(QObject *parent = nullptr);

private:
    int initialBaseline;
    int currentBaseline; //to keep track of the current baseline
    int finalBaseline;
};

#endif // ELECTRODE_H
