#include "CentralProcessor.h"
#include <iostream>
#include <fstream>

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
        //emit graphUpdate(electrodes[i]->get_beta_amp(), electrodes[i]->get_beta_freq());
    }
}


void CentralProcessor::applyFullTreatment(){
    // Calculate the baseline frequency before treatments
    double startingBaseline = calculateBaselineFrequency();
    
    // Print results to console / graph
    
    
    // Apply 4 rounds of treatment, each time recalculating the dominant frequencies
    for (int i = 0; i < numTreatments; i++){
    	applyTreatmentRound();
    }
    
    
    double endingBaseline = calculateBaselineFrequency();
    
    // Print results to console / graph    
}

//To save the current session log, todo: Ashneet
void CentralProcessor::saveData(){
    fstream dataFile;
    dataFile.open("savedLogs.txt",fstream::app);

    if(!dataFile){
        dataFile.open("savedLogs.txt",fstream::out);
    }

    dataFile.close();
}
