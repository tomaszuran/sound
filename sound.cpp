#include "sound.h"

using namespace std;

Wave::Wave(string filename)
{
    fp = fopen(filename.c_str(), "rb");

    fseek(fp, 0, SEEK_END);
    long int max = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    while(ftell(fp) < max)
    {
        char id[5];
        uint32_t size;
        fread(id, sizeof(char), 4, fp);
        id[4] = 0;

        fread(&size, sizeof(uint32_t), 1, fp);

        uint8_t check_id = checkId(id);

        if(check_id == 1) // fmt
        {
            strncpy(fmt.SubChunk1ID, id, 4); //fmt.SubChunk1ID = id;
            fmt.SubChunk1Size = size;
            fread(&fmt.AudioFormat, 1, size, fp);
        }
        else if(check_id == 2) // data
        {
            strncpy(dat.SubChunk2ID, id, 4); //dat.SubChunk2ID = id;
            dat.SubChunk2Size = size;
            dat.size = size / sizeof(short);         
            data = new signed short[dat.size];
            for(int i = 0; i < dat.size; i++)
            {
                fread(&data[i], sizeof(short), dat.size, fp);
            }
        }
        else if(check_id == 3) // riff
        {
            strncpy(RIFF.ChunkID, id, 4); //RIFF.ChunkID = id;
            RIFF.ChunkSize = size;
            fread(RIFF.Format, sizeof(char), 4, fp);
        }
        else
        {
            cout << "CheckID error" << endl;
            cout << "Ftell: " << ftell(fp) << endl;
        }
    }
}

Wave::~Wave()
{
    fclose(fp);
    delete[] data;
}

void Wave::printRIFF()
{
    cout << "Riff:" << endl;

    char aux[5];
    for(int i=0; i<4; i++)
        aux[i] = RIFF.ChunkID[i];
    aux[4] = '\0';

    cout << aux << endl;

    cout << RIFF.ChunkSize << endl;

    for(int i=0; i<4; i++)
        aux[i] = RIFF.Format[i];
    aux[4] = '\0';

    cout << aux << endl;
}

void Wave::printFMTS()
{
    cout << "FMT:" << endl;
    cout << "Audio Format: " << fmt.AudioFormat << endl;
    cout << "NumChannels: " << fmt.NumChannels << endl;
    cout << "SampleRate: " << fmt.SampleRate << endl;
    cout << "ByteRate: " << fmt.ByteRate << endl;
    cout << "BlockAlign: " << fmt.BlockAlign << endl;
    cout << "BitsPerSample: " << fmt.BitsPerSample << endl;
}

void Wave::printDATA()
{
     for(int j = 0; j < dat.size; j++)
    {
        cout << data[j] << endl;
    }
}

uint8_t Wave::checkId(char * id)
{
    //check fmt 
    uint8_t check_fmt = 1;
    uint8_t check_data = 1;
    uint8_t check_riff = 1;

    char fmt[] = "fmt ";
    char data[] = "data";
    char riff[] = "RIFF";
    for(int i = 0; i < 4; i++)
    {
        if(id[i] != fmt[i])
            check_fmt = 0;
        if(id[i] != data[i])
            check_data = 0;
        if(id[i] != riff[i])
            check_riff = 0;        
    }

    if(check_fmt)
        return 1;
    else if(check_data)
        return 2;
    else if(check_riff)
        return 3;
    else
        return 0;
}

Wave Wave::createWave(std::string filename, WAVE_RIFF riff, WAVE_FMT fmt, int duration_ms)
{
    FILE *fp = fopen(filename.c_str(), "wb");
    
    fwrite(&riff, sizeof(WAVE_RIFF), 1, fp);
    fwrite(&fmt, sizeof(WAVE_FMT), 1, fp);

    WAVE_DATA dat;

    strncpy(dat.SubChunk2ID, "data", 4);

    uint32_t NumSamples = duration_ms * 1000 * fmt.SampleRate;

    dat.SubChunk2Size = NumSamples * fmt.BlockAlign;

    fwrite(dat.SubChunk2ID, sizeof(char), 4, fp);
    fwrite(&dat.SubChunk2Size, sizeof(uint32_t), 1, fp);

    short data[NumSamples];

    for(int i = 0; i < NumSamples; i++)
        data[i] = 0;

    fwrite(data, sizeof(short), NumSamples, fp);
    
    fclose(fp);


    Wave r(filename.c_str());

    return r;
}

void Wave::saveAs(std::string filename)
{
    FILE *newfile = fopen(filename.c_str(), "wb");
    fwrite(&RIFF, sizeof(WAVE_RIFF), 1, newfile);
    fwrite(&fmt, sizeof(WAVE_FMT), 1, newfile);
    fwrite(dat.SubChunk2ID, sizeof(char), 4, newfile);
    fwrite(&dat.SubChunk2Size, sizeof(uint32_t), 1, newfile);
    fwrite(data, sizeof(short), dat.size, newfile);
    fclose(newfile);
}