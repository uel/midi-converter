#include "SysexEvent.h"

SysexEvent::SysexEvent(MidiFileSegment& s, uint32_t absT, uint8_t status, std::vector<uint8_t> data)
: Event(s, absT, status)
{
    this->absT = absT;
    this->status = status;
    for (auto i : data)
        this->data.push_back(i);    
}

void SysexEvent::Read()
{
    size_t contentLength = ReadVLQ(&length);
    for (size_t i = 0; i < contentLength; i++) {
        data.push_back(iFile->get());
    }
    length += contentLength;
}

void SysexEvent::Write() const
{
    WriteVLQ(deltaT);
    oFile->put((char)status);
    WriteVLQ(data.size());
    for (auto i : data)
        oFile->put((char)i);
}