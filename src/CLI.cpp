#include "CLI.h"
#include <fstream>
#include <iostream>
#include "Visualization.h"
#include "MidiPath.h"
#include "JsonPath.h"

enum class Argument{ j, m, r, t, i, s, other };
/** Converts string arguments to a number for usage in switch statement */
Argument GetArg(std::string arg)
{
    if ( arg == "-j") return Argument::j;
    if ( arg == "-m") return Argument::m;
    if ( arg == "-r") return Argument::r;
    if ( arg == "-t") return Argument::t;
    if ( arg == "-i") return Argument::i;
    if ( arg == "-s") return Argument::s;
    return Argument::other;
}

CLI::CLI(size_t argc, char *argv[])
{
    //Special case for visual mode
    if ( argc == 3  && argv[1] == std::string("-v"))
    {   
        try 
        {
            MidiJson m = MidiFile(argv[2], false).Parse();
            Visualization::Start(m, argc, argv);
        } 
        catch ( const char* c )
        {
            std::cout << c << std::endl;
        }
        
        return;
    }

    //Non visual mode
    if ( !ReadArguments(argc, argv) )
    {
        PrintUsage();
        return;
    }
    
    if ( !ValidateArguments())
        return;

    Run();
}

void CLI::PrintUsage() const
{
 std::string u = "\n\
    Usage:\n\
    -j [input_files] - JSON mode: convert from json to midi\n\
    -m [input_files] - MIDI mode: convert from midi to json\n\
    -r [-j ^ -m] root_directories - RECURSIVE mode: convert json or midi files recursively\n\
    -v midi_file - VISUAL mode: starts piano roll visualization of midi file\n\
\n\
    -s suffix - appends suffix to output files\n\
    -t tempo - specifies the tempo of the .mid output, only applied with -j\n\
    -i instrument_number - specifies the instrument of the .mid output, only applied with -j\n\n";
 std::cout << u;
}

bool CLI::ReadArguments(size_t argc, char *argv[])
{
    if ( argc < 2 )
    {
        std::cout << "Wrong usage" << std::endl;
        return false;
    }

    for (size_t i = 1; i < argc;)
    {
        switch (GetArg(argv[i]))
        {
        case Argument::j:
            while (++i < argc && GetArg(argv[i]) == Argument::other)
                files.push_back(std::make_shared<JsonPath>(argv[i], *this));
            if ( !files.size() )
            {
                std::cout << "No json files specified" << std::endl;
                return false;
            }
            break;
        case Argument::m:
            while (++i < argc && GetArg(argv[i]) == Argument::other)
                files.push_back(std::make_shared<MidiPath>(argv[i], *this));
            if ( !files.size() )
            {
                std::cout << "No midi files specified" << std::endl;
                return false;
            }
            break;
        case Argument::r:
            recursive = true;
            i++;
            break;
        case Argument::t:
            i++;
            if ( i < argc )
                tempo = atoi(argv[i]);
            else
            {
                std::cout << "No tempo specified" << std::endl;
                return false;
            }
            i++;
            break;
        case Argument::i:
            i++;
            if ( i < argc )
                instrument = atoi(argv[i]);
            else
            {
                std::cout << "No instrument specified" << std::endl;
                return false;
            }
            i++;
            break;
        case Argument::s:
            i++;
            if ( i < argc )
                suffix = argv[i];
            else
            {
                std::cout << "No suffix specified" << std::endl;
                return false;
            }
            i++;
            break;
        default:
            std::cout << "Wrong usage!" << std::endl;
            return false;
        }
    }

    return true;
}

bool CLI::Run()
{
    for (auto i : files)
        i->Convert();
   
    if (files.size() == 0)
        std::cout << "No files found" << std::endl;

    return true;
}

bool CLI::ValidateArguments() const
{
    if ( instrument < 0 || instrument > 127 )
    {
        std::cout << "Instrument must be between 0 and 127 " << std::endl;
        return false;
    } 

    if ( tempo < 1 || tempo > 1024 )
    {
        std::cout << "Tempo must be between 1 and 1024 " << std::endl;
        return false;
    } 

    for (auto i : files)
    {
        if ( !i->Validate() )
            return false;
    }
    return true;
}