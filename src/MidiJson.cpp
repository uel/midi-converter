#include "MidiJson.h"
#include "MidiFile.h"
#include <iostream>
#include <fstream>
#include "string.h"



MidiJson::MidiJson(const std::string& path)
{
    Load(path);
}

void MidiJson::Save(const std::string& path) const
{
    std::ofstream out(path, std::ios::out|std::ios::trunc);

    if (!out.is_open())
        throw "Unable to create output JSON file";
    
    size_t k = 0;
    out << "[\n";
    for (auto i : notes)
    {
        out << "{ \"note\": "<< (int)i.note <<" , \"start\": " << i.begin ;
        out << ", \"duration\": " << i.duration << ", \"velocity\": " << (int)i.velocity << " }";
        if ( k != notes.size()-1 )
            out << ",";
        out << std::endl;
        k++;
    }
    out << "]\n";
}

char FirstNonWSChar(std::ifstream& in)
{
    char c = in.get();
    while (std::isspace(c))
        c = in.get();
    return c;
}

double ReadPair(std::ifstream& in, std::string key, bool readComma)
{
    char s[20];
    if ( FirstNonWSChar(in) != '"' ) throw "Invalid midi JSON format";
    in.read(s, key.length()+1);
    if ( strncmp(s, (key+"\"").c_str(), 5) )  throw "Invalid midi JSON format";
    if ( FirstNonWSChar(in) != ':' ) throw "Invalid midi JSON format";
    double t;
    if ( !(in >> t) ) throw "Invalid midi JSON format";
    if ( readComma && (FirstNonWSChar(in) != ',' )) 
        throw "Invalid midi JSON format";
    return t;
}

void MidiJson::Load(const std::string& path)
{
    std::ifstream in(path, std::ios::in);
    if (!in.is_open())
        throw "Unable to create output JSON file";

    char c = FirstNonWSChar(in);
    if ( c != '[' ) throw "Invalid midi JSON format";
    while (1)
    {
        Note n;
        c = FirstNonWSChar(in);
        if ( c == ']' )
            break;
        else if ( c != '{' ) throw "Wrong format";

        n.note = ReadPair(in, "note", true);
        n.begin = ReadPair(in, "start", true);
        n.duration = ReadPair(in, "duration", true);
        n.velocity = ReadPair(in, "velocity", false);

        if ( FirstNonWSChar(in) != '}' ) throw "Wrong format";
        c = FirstNonWSChar(in);
        notes.push_back(n);
        if ( c == ',' ) continue;
        else if ( c == ']' ) break;
        else throw "Wrong format";
    }

    return;
}
