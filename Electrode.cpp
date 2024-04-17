#include "Electrode.h"

Electrode::Electrode(QObject *parent):QObject(parent){
    // Create a random number generator
    random_device dev;
    mt19937 rng(dev());
  
    uniform_int_distribution<int> amp(1,5);
    uniform_real_distribution<double> aFreq(8,12);
    uniform_real_distribution<double> bFreq(12,30);
    uniform_real_distribution<double> dFreq(1,4);
    uniform_real_distribution<double> tFreq(4,8);

    // Generate a random number for the amplitudes in the range 0 to 5 (non inclusive)
    alpha_amp = amp(rng);
    beta_amp = amp(rng);
    delta_amp = amp(rng);
    theta_amp = amp(rng);

    // Generate a random frequency within the range defined for each wave
    alpha_freq = aFreq(rng);
    beta_freq = bFreq(rng);
    delta_freq = dFreq(rng);
    theta_freq = tFreq(rng);

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

