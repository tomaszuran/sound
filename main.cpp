#include <iostream>
#include "sound.h"
#include <math.h>

#define PI 3.141592653589793

int sine(float t);

int main(int argc, char **argv)
{
    std::cout << "Wave reader" << std::endl;
    
    Wave wave("test.wav");

    wave.printFMTS();

    //muestreo a 44100 Hz durante 3 segs

    for(float i = 0; i < wave.getDataSize(); i++)
    {
        float t = (1.0 / (float)wave.getFormat().SampleRate) * i;
        wave.setData(i, sine(t));
    }

    wave.saveAs("test2.wav");

    return 0;
}

int sine(float t)
{
    return (int)(30000.0*sin(2*PI*2000*t));
}