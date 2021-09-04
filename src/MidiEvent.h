#include "Event.h"

///MidiEvent is any event affecting a channel, status > 0x80
struct MidiEvent : public Event
{
    uint8_t channel;        ///< Channel of the midi event, 4 bits.
    uint8_t data[2] = {0};  ///< Data is 1 or 2 bytes long depeding on type
    
    enum class Type { NoteOn = 0b1001, NoteOff = 0b1000, Pedal = 0b1011 };
    Type type;  ///< The type of the MidiEvent object

    /** Initializes a Midi event from the iFile stream. */
    MidiEvent(MidiFileSegment& s, int8_t b = 0);

    /** Initializes a Midi event from it's member variables. */
    MidiEvent(MidiFileSegment& s, uint32_t absT, uint8_t status, const uint8_t* data, uint32_t length);

    /** Reads a Midi event from iFile. */
    void Read();    

    /** Writes the Midi event into the oFile stream. */
    void Write() const override;

    /** Return the note number. Only valid if type is note on/off */
    uint8_t note() const { return data[0]; }   

    /** Return velocity strength. Only valid if type is note on/off */ 
    uint8_t velocity() const { return data[1]; }
};
