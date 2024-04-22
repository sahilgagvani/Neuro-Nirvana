#include "CentralProcessor.h"
#include <iostream>
#include <fstream>
#include<unistd.h>

using namespace std;


CentralProcessor::CentralProcessor(QObject *parent):QObject(parent){
    // Initialize all the electrodes
    for (int i = 0; i < numElectrodes; i++){
        electrodes[i] = new Electrode();
    }
}

CentralProcessor::~CentralProcessor(){
    // Free all the allocated memory
    for (int i = 0; i < numElectrodes; i++){
        delete electrodes[i];
    }
}


// Baseline frequency is the average of all dominant frequencies
double CentralProcessor::calculateBaselineFrequency(){
    double sumOfFrequencies = 0;

    for (int i = 0; i < numElectrodes; i++){
        sumOfFrequencies += electrodes[i]->calculateDominantFrequency();
    }

    return sumOfFrequencies / numElectrodes;
}


double CentralProcessor::calculateBaselineAmplitude(){
    double sumOfAmplitudes = 0;

    for (int i = 0; i < numElectrodes; i++){
        sumOfAmplitudes += electrodes[i]->getAverageAmplitude();
    }

    return sumOfAmplitudes / numElectrodes;
}



// Apply ONE ROUND of treatment
void CentralProcessor::applyTreatmentRound(){
    double newOffsetFreq = offsetFreq / 16;
    // Loop over all electrodes to deliver treatment
    for (int i = 0; i < numElectrodes; i++){

        // Apply the 1/16th of the offset to each electrodes frequencies
        for (int j = 0; j < 16; j++){
            electrodes[i]->applyOffset(newOffsetFreq);
        }

        // Calculate the new dominant frequency for the electrode
        // Update that variable in the electrode object
        double newDomFreq = electrodes[i]->calculateDominantFrequency();
        electrodes[i]->set_dominant_freq(newDomFreq);

        emit graphUpdate(electrodes[i]->get_alpha_amp(), newDomFreq);
        qInfo("Added offset of %.2f hz to electrode %d", offsetFreq, i+1);
    }
}

void CentralProcessor::singleElectrodeGraph(int index) {
    if (index < 0) {
        emit graphUpdate(electrodes[0]->get_alpha_amp(), electrodes[0]->get_dominant_freq());
    } else {
        emit graphUpdate(electrodes[index]->get_beta_amp(), electrodes[index]->get_beta_freq());
    }
}

void CentralProcessor::applyFullTreatment(){
    // Calculate the baseline frequency before treatments
    double startingBaseline = calculateBaselineFrequency();
    double baselineAmplitude = calculateBaselineAmplitude();

    // Update display and log changes
    emit graphUpdate(startingBaseline, baselineAmplitude);
    qInfo("Initial baseline frequency: %.2fhz", startingBaseline);
    qInfo("\nPerforming Calculations. Please wait for 5 seconds..");
    sleep(5);
    // Apply 4 rounds of treatment, each time recalculating the dominant frequencies
    for (int i = 0; i < numTreatments; i++){
        qInfo("\Performing treatment. Please wait for 1 second..");
        sleep(1);
        qInfo("\nStarting round %d of %d treatments", i+1, numTreatments);
        applyTreatmentRound();
        qInfo("Ending round %d of %d treatments", i+1, numTreatments);
    }

    double endingBaseline = calculateBaselineFrequency();
    qInfo("\nCalculating baseline frequency. Please wait for 5 seconds..");
    sleep(5);

    // Print results to console / graph
    emit graphUpdate(endingBaseline, baselineAmplitude);
    emit sendSaveData(startingBaseline, endingBaseline);

    qInfo("\nFinal baseline frequency: %.2fhz", endingBaseline);
}
