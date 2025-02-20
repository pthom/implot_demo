#include "hello_imgui/hello_imgui.h"
#include "hello_imgui/icons_font_awesome_4.h"
#include "implot.h"
#include "implot3d.h"
#include "ImGuiExt.h"
#include "TextEditor.h"
#include "LibrarySources.h"
#include "MarkdownHelper.h"
#include "JsClipboardTricks.h"
#include <fplus/fplus.hpp>

#include "HyperlinkHelper.h"


void guiCodeRegions(const LinesWithNotes &linesWithNotes, TextEditor &editor)
{
    for (const auto & lineWithNote : linesWithNotes) {
        if (ImGui::Button(lineWithNote.second.c_str()))
            editor.SetCursorPosition({lineWithNote.first, 0}, 3);
        ImGuiExt::SameLine_IfPossible();
    }
    ImGui::NewLine();
}


void setEditorAnnotatedSource(const AnnotatedSourceCode& annotatedSourceCode, TextEditor& editor)
{
    editor.SetText(annotatedSourceCode.sourceCode);
    std::unordered_set<int> lineNumbers;
    for (auto kv : annotatedSourceCode.linesWithNotes)
        lineNumbers.insert(kv.first);
    editor.SetBreakpoints(lineNumbers);
}

#ifdef __EMSCRIPTEN__
void handleJsClipboardShortcuts(TextEditor& textEditor)
{
  ImGuiIO& io = ImGui::GetIO();
  auto shift = io.KeyShift;
  auto ctrl = io.ConfigMacOSXBehaviors ? io.KeySuper : io.KeyCtrl;
  auto alt = io.ConfigMacOSXBehaviors ? io.KeyCtrl : io.KeyAlt;

  bool shallFillBrowserClipboard = false;
  if (ctrl && !shift && !alt && ImGui::IsKeyPressed(ImGuiKey_Insert))
      shallFillBrowserClipboard = true;
  else if (ctrl && !shift && !alt && ImGui::IsKeyPressed(ImGuiKey_C))
      shallFillBrowserClipboard = true;
  else if (ctrl && !shift && !alt && ImGui::IsKeyPressed(ImGuiKey_X))
      shallFillBrowserClipboard = true;
  else if (!ctrl && shift && !alt && ImGui::IsKeyPressed(ImGuiKey_Delete))
      shallFillBrowserClipboard = true;

  #ifdef IMGUIMANUAL_CLIPBOARD_EXPORT_TO_BROWSER
  if (shallFillBrowserClipboard)
  {
      std::cout << "Should copy!!!" << "\n";
      JsClipboard_SetClipboardText(textEditor.GetSelectedText().c_str());
  }
  #endif
}
#endif // #ifdef __EMSCRIPTEN__



struct TextEditorAnnotatedSource
{
    AnnotatedSourceCode annotatedSource;
    TextEditor editor;

    TextEditorAnnotatedSource(const char *source_path)
    {
        annotatedSource = ReadSelectedLibrarySource(source_path);

        editor.SetPalette(TextEditor::GetLightPalette());

        bool isPython = fplus::is_suffix_of<std::string>(".py", source_path);
        auto lang =
            isPython ? TextEditor::LanguageDefinition::Lua() :TextEditor::LanguageDefinition::CPlusPlus();
        editor.SetLanguageDefinition(lang);
        setEditorAnnotatedSource(annotatedSource, editor);
    }

    void Gui()
    {
        guiCodeRegions(annotatedSource.linesWithNotes, editor);

        #ifdef __EMSCRIPTEN__
        if (ImGui::SmallButton("Copy " ICON_FA_COPY))
            JsClipboard_SetClipboardText(editor.GetSelectedText().c_str());
        #endif

        editor.Render(annotatedSource.sourcePath.c_str());

        #ifdef __EMSCRIPTEN__
        handleJsClipboardShortcuts(editor);
        #endif
    }
};



int main(int, char **)
{
    TextEditorAnnotatedSource implotEditorAnnotatedSource("implot/implot_demo.cpp");
    TextEditorAnnotatedSource implot3dEditorAnnotatedSource("implot3d/implot3d_demo.cpp");

    HelloImGui::RunnerParams runnerParams;

    // App window params
    runnerParams.appWindowParams.windowTitle = "implot demo - implot3d demo";
    runnerParams.appWindowParams.windowGeometry.size = { 1200, 800};

    // ImGui window params
    runnerParams.imGuiWindowParams.defaultImGuiWindowType =
            HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;
    runnerParams.imGuiWindowParams.showMenuBar = false;
    runnerParams.imGuiWindowParams.showStatusBar = true;

    // Split the screen in two parts
    runnerParams.dockingParams.dockingSplits = {
        //{ "MainDockSpace", "CodeSpace", ImGuiDir_Up, 0.5 },
    };

    // Dockable windows definitions
    HelloImGui::DockableWindow implotDock;
    {
        implotDock.label = "ImPlot Demo";
        implotDock.dockSpaceName = "MainDockSpace";
        implotDock.GuiFunction = [&implotDock] {
            if (implotDock.isVisible)
                ImPlot::ShowDemoWindow(nullptr);
        };
        implotDock.callBeginEnd = false;
    };

    HelloImGui::DockableWindow implot3dDock;
    {
        implot3dDock.label = "ImPlot3D Demo";
        implot3dDock.dockSpaceName = "MainDockSpace";
        implot3dDock.GuiFunction = [&implot3dDock] {
            if (implot3dDock.isVisible)
                ImPlot3D::ShowDemoWindow(nullptr);
        };
        implot3dDock.callBeginEnd = false;
    };


    HelloImGui::DockableWindow implotDemoCodeDock;
    {
        implotDemoCodeDock.label = "Implot Demo - Code";
        implotDemoCodeDock.dockSpaceName = "MainDockSpace";
        implotDemoCodeDock.GuiFunction = [&implotEditorAnnotatedSource] {
            ImGui::Text("View on GitHub:");
            ImGui::SameLine();
            if (ImGui::Button("C++ demo code"))
                HyperlinkHelper::OpenUrl("https://github.com/epezent/implot/blob/master/implot_demo.cpp");

            implotEditorAnnotatedSource.Gui();
        };
    }

    HelloImGui::DockableWindow implot3dDemoCodeDock;
    {
        implot3dDemoCodeDock.label = "Implot3d Demo - Code";
        implot3dDemoCodeDock.dockSpaceName = "MainDockSpace";
        implot3dDemoCodeDock.GuiFunction = [&implot3dEditorAnnotatedSource] {
            ImGui::Text("View on GitHub:");
            ImGui::SameLine();
            if (ImGui::Button("C++ demo code"))
                HyperlinkHelper::OpenUrl("https://github.com/brenocq/implot3d/blob/main/implot3d_demo.cpp");
            ImGui::SameLine();
            if (ImGui::Button("Python demo code"))
                HyperlinkHelper::OpenUrl("https://github.com/pthom/imgui_bundle/blob/main/bindings/imgui_bundle/demos_python/demos_implot3d/implot3d_demo.py");

            implot3dEditorAnnotatedSource.Gui();
        };
    }

    // Fonts
    runnerParams.callbacks.LoadAdditionalFonts = MarkdownHelper::LoadFonts;

    // Set app dockable windows
    runnerParams.dockingParams.dockableWindows = {
        implotDock, implot3dDock, implotDemoCodeDock, implot3dDemoCodeDock };

    auto implotContext = ImPlot::CreateContext();
    auto implot3dContext = ImPlot3D::CreateContext();

    HelloImGui::Run(runnerParams);

    ImPlot::DestroyContext(implotContext);
    ImPlot3D::DestroyContext(implot3dContext);

    return 0;
}
