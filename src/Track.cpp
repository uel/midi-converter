#include <iomanip>
#include <algorithm>

#include "Track.h"
#include "string.h"
#include "Event.h"
#include "MidiEvent.h"
#include "MetaEvent.h"
#include "SysexEvent.h"
#include "MidiFileSegment.h"


std::shared_ptr<Event> Track::ReadEvent(uint32_t& absT)
{
    uint32_t deltaT = ReadVLQ(&length);
    uint8_t b = iFile->get();
    if ( *runningStatus == 0 || b >> 7)
    {
        *runningStatus = b;
        b = 0;
    }
    std::shared_ptr<Event> e;
    
    if ( *runningStatus == 0xFF )
    {
        e = std::make_shared<MetaEvent>(*this);
        *runningStatus = 0;     //Meta and sysex events reset running status
    }
    else if ( *runningStatus == 0xF7 || *runningStatus == 0xF0 )
    {
        e = std::make_shared<SysexEvent>(*this);
        *runningStatus = 0;
    }
    else
        e = std::make_shared<MidiEvent>(*this, b);

    e->deltaT = deltaT;
    absT += deltaT;
    e->absT = absT;

    return e;
}

void Track::Read()
{
    char chunkType[4];
    iFile->read(chunkType, 4);
    if ( strncmp(chunkType, "MTrk", 4) )
    {
        length = 0;
        return;
    }

    length = ReadInt(4);

    uint32_t absT = 0;
    do
    {
        events.push_back( ReadEvent(absT) );

        if ( verbose ) 
        {
            std::cout << std::setw(3) << events.back()->deltaT << " ";
            std::cout << std::hex << (int)*runningStatus;
            std::cout << std::endl;
        }

        if (events.back()->status == 0xFF)  // Check for end of track event
        {
            auto meta = std::static_pointer_cast<MetaEvent>(events.back());
            if ( meta->type == MetaEvent::Type::EndOfTrack )
                break;
        }
    } while (!iFile->eof());
    
    if ( iFile->eof() )
        std::cout << "Faulty midi format, missing EndOfTrack" << std::endl;
}

void Track::Write() const
{
    char header[] = {0x4D, 0x54, 0x72, 0x6B, 0, 0, 0, 21};
    oFile->write(header, 8);

    for (auto i : events)
        i->Write();

    uint32_t tLength = (int)oFile->tellp()-18;
    oFile->seekp(18);   // Return to position 18 to write track length
    WriteInt(tLength);
    oFile->seekp(0, std::ios::end);
}

///Microseconds per beat to 3 byte array
std::vector<uint8_t> TempoToBytes(uint32_t n, uint32_t byteCount)
{
    n = (60/(float)n)*1000000;
    std::vector<uint8_t> res;
    for (size_t i = 0; i < byteCount; i++)
        res.insert(res.begin(), ( n >> i*8 ) & 0b11111111);
    return res;
}

Track::Track(MidiFileSegment& s, MidiJson& m, uint32_t tempo, uint8_t instrument) : MidiFileSegment(s)
{
    events.push_back(std::make_shared<MetaEvent>
    (s, 0, 0xFF, MetaEvent::Type::Tempo, TempoToBytes(tempo, 3)));
    events.push_back(std::make_shared<MidiEvent>(s, 0, 0xc0, &instrument, 1));
    uint32_t end = 0;
    for (auto i : m.notes)
    {
        uint32_t absTstart = Seconds2TickPerBeat(i.begin);
        uint32_t absTend = Seconds2TickPerBeat(i.begin+i.duration);
        uint8_t onData[] = {i.note, i.velocity};
        events.push_back(std::make_shared<MidiEvent>(s, absTstart, 0x90, onData, 2));
        uint8_t offData[] = {i.note, 0};
        events.push_back(std::make_shared<MidiEvent>(s, absTend, 0x80, offData, 2));
        end = absTend;
    }
    
    std::sort(events.begin(), events.end(), 
    [](std::shared_ptr<Event>& a, std::shared_ptr<Event>& b){return a->absT < b->absT;});
    
    events.push_back(std::make_shared<MetaEvent>(s, end+2*tempo, 0xFF, MetaEvent::Type::EndOfTrack, std::vector<uint8_t>()));

    uint32_t lastabsT = 0;
    for (auto& i : events)
    {
        i->deltaT = i->absT - lastabsT;
        lastabsT = i->absT;
    }
    
    return;
}
