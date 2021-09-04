#include "JsonPath.h"
#include "MidiFile.h"
#include <filesystem>
#include <iostream>
#include "MidiJson.h"

bool JsonPath::Validate() const
{
    if ( !std::filesystem::exists(path) )
    {
        std::cout << path << " does not exist" << std::endl;
        return false;
    }

    if ( std::filesystem::is_regular_file(path) )
    {
        std::ifstream f(path);
        char c = FirstNonWSChar(f);
        f.close();

        if ( c != '[' )
        {
            std::cout << path << " is not a valid json-midi file" << std::endl;
            return false;
        }
    }

    return true;
}

void JsonPath::ConvertFile(std::string filepath)
{
    std::string nName = ExtensionReplace(filepath, ".mid");
    try {
        auto mid = MidiJson(filepath);
        MidiFile(mid, cli.tempo, cli.instrument).Save(nName);
        std::cout << nName << " <-- " << filepath << std::endl;
    } 
    catch ( const char* c )
    {
        std::cout << c << std::endl;
    }
}