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

int main() {
    short data[SAMPLES_NUMBER];

    // 5e
    Wav wav("5e.wav");
    createBaseWav(data);
    wav.write(data);

    return 0;
}
