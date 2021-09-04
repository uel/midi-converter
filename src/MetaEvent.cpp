#include "MetaEvent.h"

MetaEvent::MetaEvent(MidiFileSegment& s, uint32_t absT, uint8_t status, Type type, std::vector<uint8_t> data)
: Event(s, absT, status), type(type)
{
    for (auto i : data)
        this->data.push_back(i);  
}

void MetaEvent::Read()
{
    size_t t = iFile->get();
    type = (Type)t;
    size_t conentLength = ReadVLQ(&length);
    length += conentLength;
    for (size_t i = 0; i < conentLength; i++) {
        data.push_back(iFile->get());
    }
}

void MetaEvent::Write() const
{
    WriteVLQ(deltaT);
    oFile->put((char)0xFF);
    oFile->put((char)type);
    WriteVLQ(data.size());
    for (auto i : data)
        oFile->put((char)i);
}