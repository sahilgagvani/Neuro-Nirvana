#include "Electrode.h"

Electrode::Electrode(QObject *parent):QObject(parent){
    srand(time(NULL));

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
}


void Electrode::applyOffset(int oVal){
    // Apply the given offset to each wave's frequency
    alpha_freq += oVal;
    beta_freq += oVal;
    delta_freq += oVal;
    theta_freq += oVal;
}
