#include "hello_imgui/hello_imgui_assets.h"
#include <fplus/fplus.hpp>
#include "LibrarySources.h"



LinesWithNotes findDemoRegions(const std::string &sourceCode)
{
    LinesWithNotes r;

    // for example, a demo line resemble to: void Demo_DragRects() {
    static std::string demoToken = "void Demo";

    auto extractDemoName = [](const std::string &codeLine) {
        std::string r = codeLine;
        r= fplus::replace_tokens<std::string>("void ShowDemo_", "", codeLine);
        r = fplus::replace_tokens<std::string>("()", "", r);
        r = fplus::replace_tokens<std::string>("{", "", r);
        r = fplus::replace_tokens<std::string>(demoToken + "_", "", r);
        r = fplus::replace_tokens<std::string>(demoToken, "", r);
        r = fplus::trim_whitespace(r);
        return r;
    };

    auto lines = fplus::split('\n', true, sourceCode);
    for (size_t line_number = 0; line_number < lines.size(); line_number++)
    {
        const std::string& line = lines[line_number];

        bool lineContainsDemoToken = line.find(demoToken) != std::string::npos;
        bool lineDefinesZeroParamFunction = (line.find("()") != std::string::npos);

        if (lineContainsDemoToken && lineDefinesZeroParamFunction)
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
