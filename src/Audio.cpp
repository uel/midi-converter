#include <SDL2/SDL.h>

size_t af = 0;
double lastFrequencies[88] = {0};
size_t lastFCount = 0;

double note2period(int noteNumber)
{
	return ( 2*M_PI*27.5*pow(2,noteNumber/(double)12) )/44100;
}

void audio_callback(void *userdata, Uint8 *stream, int len) 
{
    bool* isActive = (bool*)userdata;

    double frequencies[88] = {0};
    double amps[88] = {};
    size_t fCount = 0;
    for (size_t i = 0; i < 88; i++)
    {
        if ( isActive[i] )
        {
            frequencies[fCount] = note2period(i);
            amps[fCount] = (0.4*16000*((44.f+88.f-i)/(88.)));
            fCount++;
        }
    }

    for (size_t a = 0; a < fCount; a++)
        amps[a] = amps[a]/fCount;
    

    len = len/2;
    Sint16* audio = (Sint16*)stream;

    for (int i = 0; i < len; i++)
    {
        audio[i] = 0;

        double trans = i > len/1.f ? 0 : cos(((1.f*i)/len)*M_PI/2);

        for (size_t f = 0; f < lastFCount; f++)
            audio[i] += sin(lastFrequencies[f]*af)*amps[f]*trans;

        for (size_t f = 0; f < fCount; f++)
            audio[i] += sin(frequencies[f]*af)*amps[f]*(1-trans);

        af++;
    }

    for (size_t f = 0; f < fCount; f++)
        lastFrequencies[f] = frequencies[f];
    lastFCount = fCount;
    
	SDL_MixAudio(stream, stream, len*2, SDL_MIX_MAXVOLUME);
}
