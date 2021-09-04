#pragma once
#include "MidiFile.h"

///Virtual class of any midi message
struct Event : public MidiFileSegment
{
    public:
    uint8_t status;         ///< Status byte of event, can be implicit from rolling status.
    uint32_t deltaT = 0;    ///< Number of ticks since last event.
    uint32_t absT = 0;      ///< Number of ticks since first event of the track.
    uint32_t length = 0;    ///< Length of event without deltaT.

    Event(MidiFileSegment& s) : MidiFileSegment(s) {}

    /** Initializes an event object from it's member variables. */
    Event(MidiFileSegment& s, uint32_t absT, uint8_t status) : MidiFileSegment(s), status(status), absT(absT) {}

    /** Reads event header from iFile. */
    void Read();

    /** Should write event to the oFile stream. */
    virtual void Write() const = 0;
};