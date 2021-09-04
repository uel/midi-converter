#pragma once
#include "MidiFileSegment.h"

class Event;
class MidiJson;

///Highest segment under the midi files itself, contains midi events.
class Track : public MidiFileSegment
{ 
    uint32_t length;    ///< Length of track content in bytes.

    /** Reads the track header and events from iFile until the end of track event or eof. 
     *  Requires iFile to be at the beggining of a Track header.*/
    void Read();

    /** Tries reading a single event from the iFile stream.*/
    std::shared_ptr<Event> ReadEvent(uint32_t& absT);

    public:
    std::vector<std::shared_ptr<Event>> events;
    
    /** Initializes a track from the iFile stream.*/
    Track(MidiFileSegment& m) : MidiFileSegment(m) { Read(); }

    /** Initializes a track from a midi object, instrument and tempo.
     * All required events are created. */
    Track(MidiFileSegment& s, MidiJson& m, uint32_t tempo, uint8_t instrument);

    /** Writes the track header and events into oFile. */
    void Write() const;
};