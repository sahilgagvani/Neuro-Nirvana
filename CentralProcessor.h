#ifndef CENTRALPROCESSOR_H
#define CENTRALPROCESSOR_H

#include "Electrode.h"
#include <QObject>

class CentralProcessor : public QObject {
    Q_OBJECT

    private:
        Electrode* electrodes[21];

        int numTreatments = 4;
        int numElectrodes = 7;
        int offsetFreq = 5;

    public:
        CentralProcessor(QObject *parent = nullptr);
        ~CentralProcessor();

        // Baseline frequency is the average of all dominant frequencies
        double calculateBaselineFrequency();
        double calculateBaselineAmplitude();

        void applyTreatmentRound();
        void applyFullTreatment();
        
        void saveData();

        Electrode* getElectrode(int index){ return electrodes[index]; }


    public slots:
        void singleElectrodeGraph(int);

    signals:
        void graphUpdate(int, int);
        void getElectrodeInfo(int);

};

#endif // CENTRALPROCESSOR_H
