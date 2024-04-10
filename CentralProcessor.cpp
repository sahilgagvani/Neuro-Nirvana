#include "CentralProcessor.h"
#include <iostream>
#include <fstream>

using namespace std;


CentralProcessor::CentralProcessor(QObject *parent):QObject(parent){
    // Initialize all the electrodes
    for (int i = 0; i < numElectrodes; i++){
        Electrode* electrode = new Electrode();
        electrodes.push_back(electrode);
    }
}

CentralProcessor::~CentralProcessor(){
    // Free all the allocated memory
    for (int i = 0; i < numElectrodes; i++){
        delete electrodes[i];
        electrodes[i] = nullptr;
    }
}

double CentralProcessor::calculateDominantFrequency(Electrode* elec){

    // Calculate the dominant frequency given the 4 waves
    double alpha_amp_squared = (elec->get_alpha_amp() * elec->get_alpha_amp());
    double beta_amp_squared = (elec->get_beta_amp() * elec->get_beta_amp());
    double delta_amp_squared = (elec->get_delta_amp() * elec->get_delta_amp());
    double theta_amp_squared = (elec->get_theta_amp() * elec->get_theta_amp());

    double alpha_calc = elec->get_alpha_freq() * alpha_amp_squared;
    double beta_calc = elec->get_beta_freq() * beta_amp_squared;
    double delta_calc = elec->get_delta_freq() * delta_amp_squared;
    double theta_calc = elec->get_theta_freq() * theta_amp_squared;

    return (alpha_calc + beta_calc + delta_calc + theta_calc) / (alpha_amp_squared + beta_amp_squared + delta_amp_squared + theta_amp_squared);
}


double CentralProcessor::calculateBaselineFrequency(){
    // I'm RELATIVELY sure that a baseline frequency is the average of all dominant frequencies
    // That's what the sepcs say, but I'll ask

    double sumOfFrequencies = 0;

    for (int i = 0; i < numElectrodes; i++){
        sumOfFrequencies += calculateDominantFrequency(electrodes[i]);
    }

    return sumOfFrequencies / numElectrodes;
}



void CentralProcessor::applyTreatmentRound(){
    // Probably implement some signals here that will send text for the main window console to print

    // Getting mixed answers on how to update each wave, so I'll wait for clarification
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
