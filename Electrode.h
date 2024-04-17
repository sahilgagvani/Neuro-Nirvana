#ifndef ELECTRODE_H
#define ELECTRODE_H

#include <QObject>
#include <cstdlib>
#include <ctime>
#include <cstdio>

class Electrode : public QObject {
    Q_OBJECT;

    private:
        int alpha_freq = 0;
        int alpha_amp = 0;

        int beta_freq = 0;
        int beta_amp = 0;

        int delta_freq = 0;
        int delta_amp = 0;

        int theta_freq = 0;
        int theta_amp = 0;
        
        double currentDominantFrequency = 0;
        double averageAmplitude = 0;


    public:
        Electrode(QObject *parent = nullptr);

        void applyOffset(double);
        
        // Calculates the dominant frequency
        double calculateDominantFrequency();

        // Getters
        int get_alpha_freq(){ return alpha_freq; }
        int get_alpha_amp(){ return alpha_amp; }

        int get_beta_freq(){ return beta_freq; }
        int get_beta_amp(){ return beta_amp; }

        int get_delta_freq(){ return delta_freq; }
        int get_delta_amp(){ return delta_amp; }

        int get_theta_freq(){ return theta_freq; }
        int get_theta_amp(){ return theta_amp; }

        double get_dominant_freq(){ return currentDominantFrequency; }
        double getAverageAmplitude(){ return averageAmplitude; }
        
        // Setters
        void set_dominant_freq(double newFreq){ currentDominantFrequency = newFreq; }


};

#endif // ELECTRODE_H
