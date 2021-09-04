#include "FilePath.h"
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>


size_t FilePath::EndsWith(std::string str, std::string end) const
{
    std::for_each(str.begin(), str.end(), [](char & c) {c = ::tolower(c);});
    auto l = str.rfind(end);
    if (l == std::string::npos || l != str.size() - end.size())
        return 0;
    return l;
}

/** Replaces "old" with "n" in "path" and appends ext before "n" */
std::string FilePath::ExtensionReplace(std::string path, std::string n) const
{
    auto loc = FilePath::EndsWith(path, extension);
    if (loc == 0)
        return path+cli.suffix+n;
    else
        return path.substr(0, loc)+cli.suffix+n;
}

/** Recursively finds all files from a root directory with a specific extension*/
std::vector<std::string> FilePath::GetDirFiles() const
{
    std::vector<std::string> result;
    if ( cli.recursive )
    {
        for(auto i = std::filesystem::recursive_directory_iterator(path); i != std::filesystem::recursive_directory_iterator(); ++i ) 
            if ( i->is_regular_file() && EndsWith(i->path(), extension))
            {   
                auto r = i->path().string();
                result.push_back(r);
            }
    }
    else
    {
        for(auto i = std::filesystem::directory_iterator(path); i != std::filesystem::directory_iterator(); ++i ) 
            if ( i->is_regular_file() && EndsWith(i->path(), extension))
            {   
                auto r = i->path().string();
                result.push_back(r);
            }
    }

    return result;
}

void FilePath::Convert()
{
    if ( std::filesystem::is_regular_file(path) )
        ConvertFile(path);
    else if ( std::filesystem::is_directory(path) )
        for (auto i : GetDirFiles())
            ConvertFile(i);
}

