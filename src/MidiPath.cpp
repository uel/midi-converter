#include "MidiPath.h"
#include "MidiFile.h"
#include <filesystem>
#include <memory>
#include <fstream>
#include "string.h"

bool MidiPath::Validate() const
{
    if ( !std::filesystem::exists(path) )
    {
        std::cout << path << " does not exist" << std::endl;
        return false;
    }

    if ( std::filesystem::is_regular_file(path) )
    {
        std::ifstream f(path);
        char header[4] = {0};
        f.read(header, 4);
        f.close();

        if ( strncmp(header, "MThd", 4) )
        {
            std::cout << path << " is not a valid midi file" << std::endl;
            return false;
        }
    }

    return true;
}

void MidiPath::ConvertFile(std::string filepath)
{
    std::string nName = ExtensionReplace(filepath, ".json");  
    try 
    {
        auto mid = MidiFile(filepath, false);
        mid.Parse().Save(nName);
        std::cout << nName << " <-- " << filepath << std::endl;
    } 
    catch ( const char* c )
    {
        std::cout << c << std::endl;
    }
}