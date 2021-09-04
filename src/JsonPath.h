#pragma once
#include "FilePath.h"
#include <memory>

///Path to Json file or folder containing json files in midi-json format.
class JsonPath : public FilePath
{
    /** Checks if file begins with a or whitepsaces '['. */
    bool Validate() const override;

    /** Converts a single json file to midi. Exceptions are written to cout. */
    void ConvertFile(std::string filepath) override;

    public:

    /** Creates a json path with a path and a CLI reference. */
    JsonPath(const std::string& path, const CLI& cli) : FilePath(path, cli) { this->extension = ".json"; }
};
