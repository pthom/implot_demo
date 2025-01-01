#pragma once
#include <vector>
#include <string>
#include <map>



using LinesWithNotes = std::map<int, std::string>;

struct AnnotatedSourceCode
{
    std::string sourcePath;
    std::string sourceCode;
    LinesWithNotes linesWithNotes;
};


// (private) LinesWithNotes findCollapsingHeaderRegions(const std::string &sourceCode);
AnnotatedSourceCode ReadSelectedLibrarySource(const std::string sourcePath);
