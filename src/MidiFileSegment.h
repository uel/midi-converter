#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

/** Converts a time duration from midi ticks to seconds */
double TickPerBeat2Seconds(double ticks, double ticksPerQuarterNote, double bpm = 120);

/** Converts a time duration from seconds to ticks per quarter note */
uint32_t Seconds2TickPerBeat(double seconds, uint32_t ticksPerQuarterNote=96, uint32_t bpm = 120);

///A parent class containing shared members and methods of midi segments 
class MidiFileSegment
{
    protected:
    bool verbose = false;   ///< Writes hex encoded bytes of each event to std::cout.
    std::shared_ptr<std::ifstream> iFile;   ///< Input file stream of a midi file.
    std::shared_ptr<std::ofstream> oFile;   ///< Output file stream which will be saved into.
    std::shared_ptr<uint8_t> runningStatus; ///< Status byte of last midi event read.

    /** Reads a variable length quantity from the iFile input stream.
     * bytesRead will be set to the size of the VLQ.
     * Returns the decoded VLQ ( max 32 bits ). */
    uint32_t ReadVLQ(uint32_t* bytesRead);

    /** Writes n encoded as variable length quantity into the output stream oFile. */
    void WriteVLQ(uint32_t n) const;

    /** Reads n bytes from the input stream iFile.
     * Returns bytes as a uint32_t */
    uint32_t ReadInt(unsigned int n);

    /** Writes n to oFile, most significant byte first*/
    void WriteInt(uint32_t n) const;

    /** Reads n bytes from iFile.
     * Returns a pointer to n bytes of dynamically allocated memory */
    uint8_t* ReadBytes(size_t nBytes);

};
