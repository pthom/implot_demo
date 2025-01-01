set(HELLOIMGUI_MACOS_NO_BUNDLE ON CACHE BOOL "" FORCE)
include(hello_imgui_add_app)

execute_process(COMMAND bash -c ${CMAKE_CURRENT_LIST_DIR}/populate_assets.sh)

set(implot_dir ${CMAKE_CURRENT_LIST_DIR}/../external/implot)
set(implot3d_dir ${CMAKE_CURRENT_LIST_DIR}/../external/implot3d)
set(textedit_dir ${CMAKE_CURRENT_LIST_DIR}/../external/ImGuiColorTextEdit)
set(fplus_dir ${CMAKE_CURRENT_LIST_DIR}/../external/FunctionalPlus/include)
set(imgui_markdown_dir ${CMAKE_CURRENT_LIST_DIR}/../external/imgui_markdown)

hello_imgui_add_app(implot_demo
    ImplotDemo.main.cpp
    LibrarySources.cpp
    LibrarySources.h
    ImGuiExt.cpp
    ImGuiExt.h
    MarkdownHelper.cpp
    MarkdownHelper.h
    HyperlinkHelper.cpp
    HyperlinkHelper.h
    JsClipboardTricks.cpp
    JsClipboardTricks.h
    ${textedit_dir}/TextEditor.h
    ${textedit_dir}/TextEditor.cpp
    #
    ${implot_dir}/implot.h
    ${implot_dir}/implot_internal.h
    ${implot_dir}/implot.cpp
    ${implot_dir}/implot_demo.cpp
    ${implot_dir}/implot_items.cpp
    #
    ${implot3d_dir}/implot3d.h
    ${implot3d_dir}/implot3d_internal.h
    ${implot3d_dir}/implot3d.cpp
    ${implot3d_dir}/implot3d_demo.cpp
    ${implot3d_dir}/implot3d_items.cpp
    ${implot3d_dir}/implot3d_meshes.cpp
    )

target_include_directories(implot_demo PRIVATE
    ${implot_dir} ${implot3d_dir} ${textedit_dir} ${fplus_dir} ${imgui_markdown_dir}
)
