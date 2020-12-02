//
// Created by Seweryn on 29.11.2020.
//

#ifndef DSP_CONSTS_H
#define DSP_CONSTS_H

// Wav info
const unsigned int FREQUENCY_RANGE = 44100;
const unsigned int CHANNELS_NUMBER = 1;
const unsigned int BYTES_PER_SAMPLE = 2;
const unsigned int DURATION = 5;
const unsigned int SAMPLES_NUMBER = DURATION * FREQUENCY_RANGE;
const unsigned int BYTE_RATE = FREQUENCY_RANGE * CHANNELS_NUMBER * BYTES_PER_SAMPLE;
const unsigned short BLOCK_ALIGNMENT = CHANNELS_NUMBER * BYTES_PER_SAMPLE;

const unsigned short SUBCHUNK_SIZE = 16;
const unsigned short PCM = 1;

const short MAX_SAMPLE_VALUE = 32767;
const double OUTPUT_FREQUENCY = 1000;

#endif //DSP_CONSTS_H