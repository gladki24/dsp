//
// Created by Seweryn on 29.11.2020.
//

#ifndef DSP_WAV_H
#include <string>
#include "Consts.h"
#define DSP_WAV_H

// Class to open and write wav file
// wav format:
// http://soundfile.sapp.org/doc/WaveFormat/
class Wav {
public:
    Wav() = delete;

    Wav(std::string fileName) : _fileName(std::move(fileName)) {}

    void write(short* data) {
        // open file to write
        FILE* wavFile = fopen(_fileName.c_str(), "wb");
        _writeHeader(wavFile);
        _writeFmtSubchunk(wavFile);
        _writeDataSubchunk(wavFile, data);
        fclose(wavFile);
    }

    void read() {
        // TODO
    }

private:
    std::string _fileName;

    // wav file has header which specifies information about wav file
    void _writeHeader(FILE* wavFile) {
        // RIFF header
        fwrite("RIFF", 1, 4, wavFile);
        _writeInLittleEndFormat(wavFile, 36 + BYTES_PER_SAMPLE * SAMPLES_NUMBER * CHANNELS_NUMBER, 4);
        // WAVE header
        fwrite("WAVE", 1, 4, wavFile);
    }

    // wav file contains fmt subchunk
    void _writeFmtSubchunk(FILE* wavFile) {
        fwrite("fmt ", 1, 4, wavFile);
        _writeInLittleEndFormat(wavFile, SUBCHUNK_SIZE, 4);
        _writeInLittleEndFormat(wavFile, PCM, 2);
        _writeInLittleEndFormat(wavFile, CHANNELS_NUMBER, 2);
        _writeInLittleEndFormat(wavFile, FREQUENCY_RANGE, 4);
        _writeInLittleEndFormat(wavFile, BYTE_RATE, 4);
        _writeInLittleEndFormat(wavFile, BLOCK_ALIGNMENT, 2);
        _writeInLittleEndFormat(wavFile, BYTES_PER_SAMPLE * 8, 2); // bits per sample 2 bytes * 8 = 16 bits
    }

    void _writeDataSubchunk(FILE* wavFile, short* data) {
        fwrite("data", 1, 4, wavFile);
        _writeInLittleEndFormat(wavFile, BYTES_PER_SAMPLE * SAMPLES_NUMBER * CHANNELS_NUMBER, 4);

        for (unsigned int i = 0; i < SAMPLES_NUMBER; i++) {
            unsigned int buffer = data[i];
            _writeInLittleEndFormat(wavFile, buffer, BYTES_PER_SAMPLE);
        }
    }

    // Some data has to be write in little endian format
    void _writeInLittleEndFormat(FILE* wavFile, unsigned int data, int bytesCount) {
        unsigned int buffer;

        // write one byte to file
        while (bytesCount > 0) {
            buffer = data & 0xFF;
            fwrite(&buffer, 1, 1, wavFile);
            bytesCount--;
            data >>= 8;
        }
    }
};


#endif //DSP_WAV_H
