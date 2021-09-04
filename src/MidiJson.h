#pragma once
#include <vector>
#include <iostream>
#include "MidiFile.h"

/** Returns the first non-whitespace character from a input file stream. */
char FirstNonWSChar(std::ifstream& in);

/// Represenation of a note with start and duration specifed in seconds.
struct Note
{
    uint8_t note;
    uint8_t velocity;
    uint8_t channel;
    double begin;
    double duration;
};

///A class to save and load musical notes to and from JSON files
struct MidiJson
{
    std::vector<Note> notes;

    /** Initializes Midi object from JSON file.*/
    MidiJson(const std::string& path);
    MidiJson(){};

    /** Saves notes into path in JSON format.*/
    void Save(const std::string& path) const;

    private:
    /** Loads notes from JSON formatted file, throws an exception if format is wrong.*/
    void Load(const std::string& path);
};