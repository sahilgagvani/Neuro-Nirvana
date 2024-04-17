#include "CentralProcessor.h"
#include <iostream>
#include <fstream>

using namespace std;


CentralProcessor::CentralProcessor(QObject *parent):QObject(parent){
    // Initialize all the electrodes
    for (int i = 0; i < numElectrodes; i++){
        electrodes[i] = new Electrode();
    }

    srand(time(NULL));
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
    
    
    // Might add a slight random offset here to make the waves different, tbd
    
    
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
    
    // Print results to console / graph
    
    
    // Apply 4 rounds of treatment, each time recalculating the dominant frequencies
    for (int i = 0; i < numTreatments; i++){
    	applyTreatmentRound();
    }
    
    
    double endingBaseline = calculateBaselineFrequency();

    emit sendSaveData(startingBaseline, endingBaseline);
    
    // Print results to console / graph    
}

