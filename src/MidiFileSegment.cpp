#include "MidiFileSegment.h"

double TickPerBeat2Seconds(double ticks, double ticksPerQuarterNote, double bpm)
{
    return (ticks/ticksPerQuarterNote)*(60/bpm);
}

uint32_t Seconds2TickPerBeat(double seconds, uint32_t ticksPerQuarterNote, uint32_t bpm)
{
    return seconds*(bpm/60)*ticksPerQuarterNote;
}

uint32_t MidiFileSegment::ReadVLQ(uint32_t* bytesRead = NULL)
{
    uint32_t res = 0;
    uint8_t b;
    for (size_t i = 0; i < 4; i++)
    {
        if (bytesRead)
            (*bytesRead)++;
        b = iFile->get();
        res = res << 7;
        res |= (b & 0b01111111);
        if (!(uint8_t)(b >> 7))
            break;
    }   

    return res;
}

void MidiFileSegment::WriteVLQ(uint32_t n) const
{
    uint8_t b[5] = {0};

    int i;
    for (i = 0; n || !i; i++)
    {
        b[i] = (n & 0b01111111) | 128*(bool)i;
        n = n >> 7;
    }   
    i--;
    for (; i >= 0; i--)
        oFile->put( b[i]);
}

void MidiFileSegment::WriteInt(uint32_t n) const
{
    for (size_t i = 0; i < 4; i++)
        oFile->put( ((char*)&n)[3-i] );     //Most significant bytes first
}

uint32_t MidiFileSegment::ReadInt(unsigned int n = 4)
{
    uint32_t res = 0;
    for (size_t i = 0; i < n; i++)
    {
        res = res << 8;
        res |= iFile->get();
    }
    return res;
}

uint8_t* MidiFileSegment::ReadBytes(size_t nBytes)
{
    char* res = new char[nBytes];
    iFile->read(res, nBytes);
    return (uint8_t*)res;
}