#pragma once
#include <string>
#include <vector>
#include <memory>
#include "CLI.h"

class CLI;
///Virtual class of a path pointing to a file or folder about to be converted.
class FilePath
{
    protected:
    const std::string path;     ///< Path to folder or file.
    const CLI& cli;             ///< Reference to the CLI object that created FilePath
    std::string extension;      ///< Searched for file extension to convert.

    /** Recursively finds all files from a root directory with a specific extension*/
    std::vector<std::string> GetDirFiles() const;
    
    /** Checks if string ends with an extension, if yes it returns the index where "end" begins
    * If "str" doesn't end with "end" or if "str' = "end", 0 is returned.*/
    size_t EndsWith(std::string str, std::string end) const;

    /** Replaces extension by newExtension in path and returns the result.
     *  If extension doesn't exist newExtension is appended to the end. */
    std::string ExtensionReplace(std::string path, std::string newExtension) const;

    /** Tries to convert onte specific file specified by filepath */
    virtual void ConvertFile(std::string filepath) = 0;

    public:
    FilePath(const std::string& path, const CLI& cli) : path(path), cli(cli) { }
    
    /** Converts file or files in directory with member options.*/
    void Convert();   

    /** Does a shallow check for the validity of the files format. */
    virtual bool Validate() const = 0;
};