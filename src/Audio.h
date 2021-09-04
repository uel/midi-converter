#include <SDL2/SDL.h>

/** Takes boolean array of length 88 as userdata.
 *  This function will be called by SDL2 when more audio data is needed.
 *  Writes len bytes of sin waves of the frequencies in the harmonic scale 
 *  represented by the userdata into stream. 1 sample is 2 bytes.*/
void audio_callback(void* userdata, Uint8* stream, int len);