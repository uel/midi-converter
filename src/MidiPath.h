#pragma once
#include "FilePath.h"

///Path to a midi file or folder containing midi files.
class MidiPath : public FilePath
{
    /** Checks if file has valid midi file header. */
    bool Validate() const override;

    /** Converts a single midi file to json-midi format. Exceptions are written to cout. */
    void ConvertFile(std::string filepath) override;

    public:
    /** Creates a midi path from with a path and a CLI reference. */
    MidiPath(const std::string& path, const CLI& cli) : FilePath(path, cli) { this->extension = ".mid"; }
};