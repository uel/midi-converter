#include "Event.h"

///MidiEvent is a miscellaneous event for track modifications and text/copyright, status = 0xFF
struct MetaEvent : public Event
{
    std::vector<uint8_t> data;  ///< Data stored by the meta event

    enum class Type { Text = 0x01, Copyright = 0x02, EndOfTrack = 0x2F, Tempo = 0x51 };
    Type type;    ///< The type of this MetaEvent object

    /** Initializes a Meta event from it's member variables. */
    MetaEvent(MidiFileSegment& s, uint32_t absT, uint8_t status, Type type, std::vector<uint8_t> data);

    /** Initializes a Meta event from the iFile stream. */
    MetaEvent(MidiFileSegment& s) : Event(s) {  this->status = *runningStatus; Read(); }

    /** Reads a MetaEvent from iFile. */
    void Read();

    /** Writes the MetaEvent to oFile. */
    void Write() const override;
};
