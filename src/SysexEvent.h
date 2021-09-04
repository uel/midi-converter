#include "Event.h"

///Represents a System exclusive event
struct SysexEvent : public Event
{
    std::vector<uint8_t> data;  ///< Variable length data stores in the sysex event.
    
    /** Initializes a sysex event from the iFile stream. */
    SysexEvent(MidiFileSegment& s) : Event(s) {  this->status = *runningStatus; Read(); }

    /** Initializes a sysex event from it's member variables. */
    SysexEvent(MidiFileSegment& s, uint32_t absT, uint8_t status, std::vector<uint8_t> data);

    /** Reads a Sysex event from iFile. */
    void Read();

    /** Writes the sysex event into the oFile stream. */
    void Write() const override;
};
