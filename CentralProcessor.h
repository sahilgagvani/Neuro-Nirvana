#ifndef CENTRALPROCESSOR_H
#define CENTRALPROCESSOR_H

#include "Electrode.h"
#include <QObject>
#include <list>

class CentralProcessor : public QObject {
    Q_OBJECT;

    private:
        std::vector<Electrode*> electrodes;

        int numTreatments = 4;
        int numElectrodes = 7;
        int offsetFreq = 5;

    public:
        CentralProcessor(QObject *parent = nullptr);
        ~CentralProcessor();

        // Dominant frequency for a single electrode
        double calculateDominantFrequency(Electrode*);

        // Baseline frequency is the average of all dominant frequencies
        double calculateBaselineFrequency();

        void applyTreatmentRound();

        void saveData();

};

#endif // CENTRALPROCESSOR_H
