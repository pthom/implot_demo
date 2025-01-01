#include "hello_imgui/hello_imgui_assets.h"
#include <fplus/fplus.hpp>
#include "LibrarySources.h"


std::vector<LibrarySources> thisLibrarySources()
{
    return
    {
        {
            "implot", "ImPlot", "https://github.com/epezent/implot",
                    {
                            "README.md",
                            "LICENSE",
                            "implot_demo.cpp",
                            "implot.h",
                            "implot.cpp",
                    }
        }
    };
}

std::vector<LibrarySources> otherSources()
{
    return {};
}


std::vector<LibrarySources> allSources()
{
    return fplus::append(thisLibrarySources(), otherSources());
}


LinesWithNotes findDemoRegions(const std::string &sourceCode)
{
    LinesWithNotes r;

    // for example, a demo line resemble to: void Demo_DragRects() {
    static std::string demoToken = "void Demo_";

    auto extractDemoName = [](const std::string &codeLine) {
        std::string r = codeLine;
        r= fplus::replace_tokens<std::string>("void ShowDemo_", "", codeLine);
        r = fplus::replace_tokens<std::string>("()", "", r);
        r = fplus::replace_tokens<std::string>("{", "", r);
        r = fplus::replace_tokens<std::string>(demoToken, "", r);
        r = fplus::trim_whitespace(r);
        return r;
    };

    auto lines = fplus::split('\n', true, sourceCode);
    for (size_t line_number = 0; line_number < lines.size(); line_number++)
    {
        const std::string& line = lines[line_number];
        if (line.find(demoToken) != std::string::npos)
            r[(int)(line_number + 1)] = extractDemoName(line);
    }
    return r;
}

AnnotatedSourceCode ReadSelectedLibrarySource(const std::string sourcePath)
{
    std::string assetPath = std::string("code/") + sourcePath;
    auto assetData = HelloImGui::LoadAssetFileData(assetPath.c_str());
    assert(assetData.data != nullptr);

    AnnotatedSourceCode r;
    r.sourcePath = sourcePath;
    r.sourceCode = std::string((const char *) assetData.data);
    r.linesWithNotes = findDemoRegions(r.sourceCode);
    HelloImGui::FreeAssetFileData(&assetData);
    return r;
}
