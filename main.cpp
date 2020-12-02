#include <iostream>
#include "PhaseState.h"
#include "Wav.h"


// create base wav from task 5e
void createBaseWav(short *data) {

    PhaseState *phaseState = new APhaseState();
    double phase = 0;
    double sampleRatio;
    double amplitude = 1;

    short factor = 1;

    for (unsigned int i = 0; i < SAMPLES_NUMBER; i++) {
        phaseState->calc(phase, factor);

        if (factor == 1) {
            if (phase > M_PI) {
                phase = M_PI;
                phaseState = phaseState->nextPi(phaseState);
            }

            if (phase < M_PI / 2 && phase > 0) {
                phaseState = phaseState->nextHalfPi(phaseState);
            }

            if (phase < 0) {
                phaseState = new APhaseState();
                phase = 0;
                factor = -1;
            }
        }

        if (factor == -1) {
            if (phase < M_PI * -1) {
                phase = M_PI * -1;
                phaseState = phaseState->nextPi(phaseState);
            }

            if (phase > M_PI / -2 && phase < 0) {
                phaseState = phaseState->nextHalfPi(phaseState);
            }

            if (phase > 0) {
                phaseState = new APhaseState();
                phase = 0;
                factor = 1;
            }
        }

        sampleRatio = amplitude * phase / M_PI;
        data[i] = static_cast<short>(sampleRatio * MAX_SAMPLE_VALUE);
    }

    delete phaseState;
}

// create file from task 6b
void modifyBaseWave(short* data) {

    // column count
    short phaseCount = 12;
    // phase length
    short phaseLength = SAMPLES_NUMBER / phaseCount;
    double amplitude = 0;


    for (unsigned i = 0; i < SAMPLES_NUMBER; i++) {
        if (i >= phaseLength * 0 && i < phaseLength * 1) {
            amplitude = 0.2;
        } else if (i >= phaseLength * 1 && i < phaseLength * 2) {
            amplitude = 0.4;
        } else if (i >= phaseLength * 2 && i < phaseLength * 3) {
            amplitude = 0.6;
        } else if (i >= phaseLength * 3 && i < phaseLength * 4) {
            amplitude = 0.8;
        } else if (i >= phaseLength * 4 && i < phaseLength * 5) {
            amplitude = 1;
        } else if (i >= phaseLength * 5 && i < phaseLength * 6) {
            amplitude = 0;
        } else if (i >= phaseLength * 6 && i < phaseLength * 7) {
            amplitude = 1;
        } else if (i >= phaseLength * 7 && i < phaseLength * 8) {
            amplitude = 0;
        } else if (i >= phaseLength * 8 && i < phaseLength * 9) {
            amplitude = 1;
        } else if (i >= phaseLength * 9 && i < phaseLength * 10) {
            amplitude = 0;
        } else if (i >= phaseLength * 10 && i < phaseLength * 11) {
            amplitude = 1;
        } else if (i >= phaseLength * 11 && i < phaseLength * 12) {
            amplitude = 0;
        }

        data[i] = data[i] * amplitude;
    }

}

int main() {

    // 5e
    short data[SAMPLES_NUMBER];
    Wav wav5e("5e.wav");
    createBaseWav(data);
    wav5e.write(data);

    // 6b
    short sampleData[SAMPLES_NUMBER];
    wav5e.read(sampleData);

    // save 6b result
    Wav wav6b ("6b.wav");
    modifyBaseWave(sampleData);
    wav6b.write(sampleData);

    return 0;
}
