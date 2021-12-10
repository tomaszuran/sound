#include <fstream>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <cstring>

typedef struct _WAVE_RIFF
{
    char ChunkID[4];
    uint32_t ChunkSize;
    char Format[4];
}WAVE_RIFF;

typedef struct _WAVE_FMT
{
    char SubChunk1ID[4];
    uint32_t SubChunk1Size;
    uint16_t AudioFormat;
    uint16_t NumChannels;
    uint32_t SampleRate;
    uint32_t ByteRate;
    uint16_t BlockAlign;
    uint16_t BitsPerSample;
    //Extra params
}WAVE_FMT;

typedef struct _WAVE_DATA
{
    char SubChunk2ID[4];
    uint32_t SubChunk2Size;
    uint32_t size;
}WAVE_DATA;

class Wave
{
    private:
        //std::fstream ws;
        FILE *fp;

        
        WAVE_RIFF RIFF;

        short * data;

        WAVE_FMT fmt;
        WAVE_DATA dat;

        uint8_t checkId(char * id);
        

    public:
        Wave(std::string filename);
        ~Wave();

        inline int getDataSize() { return dat.size; }

        inline short getData(int n) { return data[n]; }

        inline void setData(int n, short val) { data[n] = val; }

        inline WAVE_FMT getFormat() { return fmt; }

        void saveAs(std::string filename);
        static Wave createWave(std::string filename, WAVE_RIFF riff, WAVE_FMT fmt, int duration_ms);

        void printRIFF();
        void printFMTS();
        void printDATA();
};