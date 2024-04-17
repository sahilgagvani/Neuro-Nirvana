#include "Electrode.h"

Electrode::Electrode(QObject *parent):QObject(parent){
    // Generate a random number for the amplitudes in the range 0 to 5 (non inclusive)
    alpha_amp = rand() % 5 + 1;
    beta_amp = rand() % 5 + 1;
    delta_amp = rand() % 5 + 1;
    theta_amp = rand() % 5 + 1;


    // Generate a random frequency within the range defined for each wave
    alpha_freq = rand() % 12 + 8;
    beta_freq = rand() % 30 + 12;
    delta_freq = rand() % 4 + 1;
    theta_freq = rand() % 8 + 4;
    
    currentDominantFrequency = calculateDominantFrequency();
    averageAmplitude = (alpha_amp + beta_amp + delta_amp + theta_amp) / 4;
}


void Electrode::applyOffset(double oVal){
    // Apply the given offset to each wave's frequency
    alpha_freq += oVal;
    beta_freq += oVal;
    delta_freq += oVal;
    theta_freq += oVal;
}

double Electrode::calculateDominantFrequency(){

    // Calculate the dominant frequency given the 4 waves following the formula we got
    double alpha_amp_squared = alpha_amp * alpha_amp;
    double beta_amp_squared = beta_amp * beta_amp;
    double delta_amp_squared = delta_amp * delta_amp;
    double theta_amp_squared = theta_amp * theta_amp;

    double alpha_calc = alpha_freq * alpha_amp_squared;
    double beta_calc = beta_freq * beta_amp_squared;
    double delta_calc = delta_freq * delta_amp_squared;
    double theta_calc = theta_freq * theta_amp_squared;

    return (alpha_calc + beta_calc + delta_calc + theta_calc) / (alpha_amp_squared + beta_amp_squared + delta_amp_squared + theta_amp_squared);
}

