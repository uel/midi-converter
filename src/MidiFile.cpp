#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <list>
#include "string.h"

#include "MidiFileSegment.h"
#include "MidiFile.h"
#include "Event.h"
#include "MidiEvent.h"
#include "MidiJson.h"

void MidiFile::Read()
{
    char chunkType[4];
    iFile->read(chunkType, 4);
    if ( strncmp(chunkType, "MThd", 4) )
        throw "Invalid midi file";
    
    length = ReadInt(4);
    if ( length != 6 )
        throw "Invalid midi file";

    format = ReadInt(2);
    trackCount = ReadInt(2);

    division = ReadInt(2);
}

MidiFile::MidiFile(const std::string& path, bool verbose = true)
{
    this->verbose = verbose;
    runningStatus = std::make_shared<uint8_t>(0);
    iFile = std::make_shared<std::ifstream>(path, std::ios::binary);
    if(iFile->bad())
    {
        throw "Error opening input midi file";
    }

    Read();

    for (size_t i = 0; i < (!format ? 1 : trackCount); i++)
    {
        Track t(*this);
        tracks.push_back(t);
    }
}

MidiFile::MidiFile(MidiJson& m, uint32_t tempo, uint8_t instrument)
{
    oFile = std::make_shared<std::ofstream>();
    tracks.push_back(Track(*this, m, tempo, instrument));
}

MidiJson MidiFile::Parse() const
{
    std::vector<std::shared_ptr<MidiEvent>> noteOns; 
    std::list<std::shared_ptr<MidiEvent>> noteOffs; 

    for ( auto j : tracks )
        for (auto i : j.events)
        {
            if (i->status >> 4 == 0b1001)
            {
                std::shared_ptr<MidiEvent> e = std::static_pointer_cast<MidiEvent>(i);
                if ( e->data[1] == 0 )
                    noteOffs.push_back(e);
                else
                    noteOns.push_back(e);
            }
            else if (i->status >> 4 == 0b1000)
                noteOffs.push_back(std::static_pointer_cast<MidiEvent>(i));
        }

    MidiJson m;

    for (auto i : noteOns)
    {
        for (auto j = noteOffs.begin(); j != noteOffs.end(); j++)
        {
            if ( (*j)->absT >= i->absT && i->note() == (*j)->note() )
            {
                double onTime = TickPerBeat2Seconds(i->absT, division);
                double offTime = TickPerBeat2Seconds((*j)->absT, division);
                Note n {i->note(), i->velocity(), i->channel, onTime, offTime-onTime};
                m.notes.push_back(n);
                noteOffs.erase(j);
                break;
            }
        }
    }

    return m;
}

void MidiFile::Save(const std::string& path) const
{
    oFile->open(path, std::ios::binary|std::ios::trunc);
    if(oFile->bad())
    {    
        throw "Error opening output file JSON file";
    }
    Write();
    oFile->close();
}
void MidiFile::Write() const
{
    char header[] = {0x4D, 0x54, 0x68, 0x64, 0, 0, 0, 6, 0, 0, 0, 1, 0, 96};
    oFile->write(header, 14);
    tracks[0].Write();
}   