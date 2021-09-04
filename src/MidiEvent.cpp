#include "MidiEvent.h"

MidiEvent::MidiEvent(MidiFileSegment& s, uint32_t absT, uint8_t status, const uint8_t* data, uint32_t length)
: Event(s, absT, status)
{
    this->absT = absT;
    this->status = status;
    this->length = length;
    for (size_t i = 0; i < length; i++)
        this->data[i] = data[i];    
}

MidiEvent::MidiEvent(MidiFileSegment& s, int8_t b) : Event(s)
{  
    this->status = *runningStatus;
    if ( b )
    {
        data[0] = b;
        length++;
    }
    Read(); 
}

void MidiEvent::Read()
{
    type = (Type)(*runningStatus >> 4);
    channel = (*runningStatus & 0b00001111);

    size_t l;
    if (*runningStatus < 0x80) //MIDI Controller Message
        l = 1;
    switch (*runningStatus >> 4)    //MIDI channel message
    {
        case 0b1000:
        case 0b1001:
        case 0b1010:
        case 0b1011:
        case 0b1110:
            l = 2;
            break;
        case 0b1100:
        case 0b1101:
            l = 1;
            break;
        default:
            throw "Unsuported MidiEvent";
            break;
    }
    
    for (;length < l; length++)
        data[length] = iFile->get();
}

void MidiEvent::Write() const
{
    WriteVLQ(deltaT);
    oFile->put((char)status);
    for (size_t i = 0; i < length; i++)
        oFile->put(data[i]);
}