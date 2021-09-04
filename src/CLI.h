#pragma once
#include <memory>
#include "FilePath.h"

class FilePath;

///The command line interface reads input from std::in and executes commands from the user
class CLI
{
    private:
    std::vector<std::shared_ptr<FilePath>> files; ///< Loaded conversion targets

    /** Reads arguments from argv into members of the CLI object.
     * Returns False if the syntax of the arguments is incorrect.*/
    bool ReadArguments(size_t argc, char *argv[]);
    
    /** Validates members of the CLI object.
     * returns False if files don't exist, or if numeric values are out of range.*/
    bool ValidateArguments() const;

    /** Runs the conversion based on the specified parameters.*/
    bool Run();
    
    /** Writes usage informatio to std::out*/
    void PrintUsage() const;
    
    public:
    bool recursive = false; ///< Indiciation if folder pointed to by path should be searched trough recursively.
    int tempo = 120;        ///< Tempo of the result midi files.
    int instrument = 1;     ///< Instrument of the result midi file
    std::string suffix;     ///< Suffix to be prepended before extension on conversion

    /** Initializes and runs the CLI using argv */
    CLI(size_t argc, char *argv[]);
};



