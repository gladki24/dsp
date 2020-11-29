#include <math.h>
#include <memory>
#include <string>
static inline void writeValue(unsigned int word, int nbytes, FILE* file)
{
    while (nbytes-- > 0)
    {
        unsigned result = word & 0xFF;
        fwrite(&result, 1, 1, file);
        word >>= 8;
    }
}
struct WFD
{ // WAV File Descriptor
    unsigned numberOfSamples;
    unsigned numberOfChannels;
    unsigned bytesPerSample;
    unsigned frequencyRange;
};
void writeDataToWAV(std::string filename, short* inputBuffer, WFD wfd)
{
    // Open file with proper flags
    FILE* file = fopen(filename.c_str(), "wb");
    // Write RIFF header - 12B
    fwrite("RIFF", 1, 4, file);
    writeValue(36 + wfd.bytesPerSample * wfd.numberOfSamples *
                    wfd.numberOfChannels, 4, file);
    fwrite("WAVE", 1, 4, file);
    // Write fmt subchunk
    fwrite("fmt ", 1, 4, file);
    // 4B
    writeValue(16, 4, file);
    // Subchunk size is 16 - 4B
    writeValue(1, 2, file);
    // PCM is format 1 - 2B
    writeValue(wfd.numberOfChannels, 2, file);
    // Write number of channels - 2B
    writeValue(wfd.frequencyRange, 4, file);
    // Write range - 4B
    writeValue(wfd.frequencyRange * wfd.numberOfChannels *
               wfd.bytesPerSample, 4, file); // Write byte rate - 4B
    writeValue(wfd.numberOfChannels * wfd.bytesPerSample, 2, file);
    // Write block alignment - 2B
    writeValue(8 * wfd.bytesPerSample, 2, file);
    // Sample - 2B
    // Write data subchunk
    fwrite("data", 1, 4, file);
    writeValue(wfd.bytesPerSample * wfd.numberOfSamples *
               wfd.numberOfChannels, 4, file);
    for (unsigned i = 0; i < wfd.numberOfSamples; i++)
        writeValue((unsigned int)(inputBuffer[i]), wfd.bytesPerSample,
                   file);
    // Close wav file
    fclose(file);
}
void readDataFromWAV(std::string filename, std::unique_ptr<short[]>&
outputBuffer, WFD& wfd, unsigned& nsamples)
{
    // Assume that input file has exactly 5s of recording
    // Open file for reading
    FILE* file = fopen(filename.c_str(), "rb");
    // Temporary buffer
    char buffer[4];
    // Read "RIFF"
    fread(buffer, 1, 4, file);
    // Read additionals
    fread(buffer, 1, 4, file);
    unsigned additionals = *reinterpret_cast<unsigned*>(buffer) - 36;
    // Read "WAVE"
    fread(buffer, 1, 4, file);
    // Read "fmt "
    fread(buffer, 1, 4, file);
    fread(buffer, 1, 4, file); // Read subchunk size
    fread(buffer, 1, 2, file); // Read format
    // Read number of channels
    fread(buffer, 1, 2, file);
    unsigned readNumberOfChannels =
            (unsigned)*reinterpret_cast<short*>(buffer);
    // Read frequency range
    fread(buffer, 1, 4, file);
    unsigned readFrequencyRange = *reinterpret_cast<unsigned*>(buffer);
    // Read byte rate
    fread(buffer, 1, 4, file);
    unsigned readByteRate = *reinterpret_cast<unsigned*>(buffer);
    // Read block alignment
    fread(buffer, 1, 2, file);
    unsigned readBlockAlignment =
            (unsigned)*reinterpret_cast<short*>(buffer);
    // Read bytes per sample
    fread(buffer, 1, 2, file);
    unsigned readBytesPerSample = (unsigned)*reinterpret_cast<short*>(buffer)
                                  / 8;
    // Read "data"
    fread(buffer, 1, 4, file);
    // Read size of the buffer
    fread(buffer, 1, 4, file);
    unsigned toRead = *reinterpret_cast<unsigned*>(buffer);
    unsigned readNumberOfSamples = (toRead / readBytesPerSample) /
                                   readNumberOfChannels;
    // Allocate buffer and read data
    outputBuffer = std::make_unique<short[]>(readNumberOfSamples);
    short* ptrToOutputBuffer = outputBuffer.get();
    for (unsigned i = 0; i < readNumberOfSamples; i++)
        fread(&ptrToOutputBuffer[i], 1, readBytesPerSample, file);
    // Update wfd
    wfd = { readNumberOfSamples, readNumberOfChannels, readBytesPerSample,
            readFrequencyRange };
    // Update number of samples
    nsamples = readNumberOfSamples;
    // Close file
    fclose(file);
}
static const double pi = 3.141592;
static const double twopi = 2.0 * pi;
static const short maxSampleValue = 32767;
void runTask5B(std::string outputFileName, const double frequency)
{
    constexpr unsigned frequencyRange = 44100; // 44.1kHZ
    constexpr unsigned duration = 5; // 5 seconds - frequency is const
    constexpr unsigned nsamples = duration * frequencyRange; // Number of samples
    const double amplitude = 1; // The "Height"
    double phase = 0; // Start from the "Middle"
    std::unique_ptr<short[]> sampBuffer =
            std::make_unique<short[]>(nsamples);
    short* sampPtrToBuffer = sampBuffer.get();
    // Run the loop to fill buffer
    for (unsigned i = 0; i < nsamples; i++)
    {
        phase = phase + (twopi * (frequency / frequencyRange));
        if (phase > pi)
            phase -= twopi;
        double sampleRatio = (amplitude * phase / pi);
        sampPtrToBuffer[i] = static_cast<short>(sampleRatio *
                                                maxSampleValue);
    }
    // Write to wav file
    WFD wfd = { nsamples, 1, 2, frequencyRange };
    writeDataToWAV(outputFileName, sampPtrToBuffer, wfd);
}
void runTask6C(std::string inputFilename, std::string outputFileName)
{
    std::unique_ptr<short[]> sampBuffer;
    WFD wfd;
    unsigned nsamples = 0;
    // Read data from wav file
    readDataFromWAV(inputFilename, sampBuffer, wfd, nsamples);
    short* sampPtrToBuffer = sampBuffer.get();
    unsigned half = wfd.frequencyRange / 2; // First part of envelope consists of columnsand lows(0.5s)
    unsigned columnLength = half / 10; // half / (5 columns + 5 lows)
    double amplitude = 1.0;
    // We assume that frequency is const for all the samples and amplitude is max = 1.0
    // In case of non const frequency (typical wav file) task requires Fourier transform(heavy math)
    // Read the frequency from the first sample
    double sampleRatio = static_cast<double>(sampPtrToBuffer[0]) / maxSampleValue;
    double amplitudeTimesPhase = sampleRatio * pi; // amplitudeTimesPhase = amplitude* phase
    double phase = amplitudeTimesPhase / 1.0; // phase = previousPhase + (twopi * (frequency / frequencyRange))
    double frequency = phase / twopi * wfd.frequencyRange;
    phase = 0;

    // Run the loop to update buffer
    for (unsigned i = 0; i < nsamples; i++)
    {
        double prevPhase = phase;
        sampleRatio = static_cast<double>(sampPtrToBuffer[i]) / maxSampleValue;
        amplitudeTimesPhase = sampleRatio * pi; // amplitudeTimesPhase = amplitude* phase
        phase = amplitudeTimesPhase / 1.0; // phase = previousPhase + (twopi * (frequency / frequencyRange))
        // <- 1s ->
        // * 0.5s * 0.5s *
        // | | | | | |||
        // | | | | | |||||
        // | | | | | |||||||
        // | | | | | |||||||||
        // |_|_|_|_|_|||||||||||
        double ratio = 0.0, height = 2.0;
        unsigned left = i % wfd.frequencyRange;
        if (left < columnLength)
            amplitude = 1.0;
        else if (left < columnLength * 2)
            amplitude = 0.0;
        else if (left < columnLength * 3)
            amplitude = 1.0;
        else if (left < columnLength * 4)
            amplitude = 0.0;
        else if (left < columnLength * 5)
            amplitude = 1.0;
        else if (left < columnLength * 6)
            amplitude = 0.0;
        else if (left < columnLength * 7)
            amplitude = 1.0;
        else if (left < columnLength * 8)
            amplitude = 0.0;
        else if (left < columnLength * 9)
            amplitude = 1.0;
        else if (left < columnLength * 10)
            amplitude = 0.0;
        else
        {
            ratio = static_cast<double>(half) / (left / 2) - 1;
            amplitude = 1.0;
            height = 2.0 * ratio;
            height = height - 1; // We want value between (-1, 1)
        }
        double sampleRatio = amplitude * phase / pi;
        //sampleRatio += sampleRatio > 0 ? -ratio : 0;
        if (sampleRatio > height)
        {
            double diff = sampleRatio - height;
            sampleRatio -= (diff * 2);
            if (sampleRatio > 1.0)
                sampleRatio = 1.0;
            else if (sampleRatio < -1.0)
                sampleRatio = -1.0;
        }
        // Set new sample, keep the same frequency!
        sampPtrToBuffer[i] = static_cast<short>(sampleRatio *
                                                maxSampleValue);
    }
    // Write to wav file
    writeDataToWAV(outputFileName, sampPtrToBuffer, wfd);
}
int main(int argc, char* argv[])
{
    if (argc != 1)
        return -1; // We expect no arguments

    // 5.b - generate 5s sound with different frequencies, we assume that frequency is const in each!
    runTask5B("sound5B_frequency_220.wav", 220);
    runTask5B("sound5B_frequency_1000.wav", 1000);
    runTask5B("sound5B_frequency_444.wav", 444);
    runTask5B("sound5B_frequency_10000.wav", 10000);

    // 6.c - "transform" sound from previous function, add the envelope
    runTask6C("sound5B_frequency_220.wav", "sound6C_frequency_220.wav");
    runTask6C("sound5B_frequency_1000.wav", "sound6C_frequency_1000.wav");
    runTask6C("sound5B_frequency_444.wav", "sound6C_frequency_444.wav");
    runTask6C("sound5B_frequency_10000.wav", "sound6C_frequency_10000.wav");

    // Return status "success"
    return 0;
}