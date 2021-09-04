#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "MidiJson.h"
#include "MidiFileSegment.h"
#include "Track.h"

class MidiJson;

///Representation of a midifile used to load, save and convert midifiles 
class MidiFile : public MidiFileSegment
{
    std::vector<Track> tracks;  ///< Track objects contained in midifile
    uint32_t length = 0;        ///< Length of midifile header, should be 6
    uint16_t format = 0;        ///< Format is 0, 1, or 2. Only 1 and 2 are supported
    uint16_t trackCount = 0;    ///< Number of tracks the midi file contains
    uint16_t division = 0;      ///< Microseconds per quarternote

    /** Reads and segments midi file from iFile stream*/
    void Read();
    /** Writes midi file to oFile stream. Called by Save.*/
    void Write() const; 

    public:
    /** Initializes a MidiFile object from a Midi object.
     *  If successful Save can be called to export the file object. */
    MidiFile(MidiJson& m, uint32_t tempo, uint8_t instrument);

    /** Initializes a MidiFile object from a file path */
    MidiFile(const std::string& path, bool verbose);

    /** Saves the MidiFile object into path
     * !!! Save will truncate any existing files !!! */
    void Save(const std::string& path) const;

    /** Parses all tracks of the MidiFile object into a Midi object.
     *  Absolute time is calculated and converted to seconds.
     *  All events except Note on / off are skipped. */
    MidiJson Parse() const;
};
