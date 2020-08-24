# Make our own version of imgui (which is based on a mix of the docking and tables branches
if (NOT EXISTS ${CMAKE_CURRENT_LIST_DIR}/imgui)
    execute_process(COMMAND bash -c ${CMAKE_CURRENT_LIST_DIR}/imgui_checkout.sh)
endif()

add_compile_definitions("ImDrawIdx=unsigned int")
set(imgui_dir ${CMAKE_CURRENT_LIST_DIR}/imgui)
add_library(imgui
    ${imgui_dir}/imgui_demo.cpp
    ${imgui_dir}/imgui_draw.cpp
    ${imgui_dir}/imgui_internal.h
    ${imgui_dir}/imgui_widgets.cpp
    ${imgui_dir}/imgui.cpp
    ${imgui_dir}/imgui.h
    )
target_include_directories(imgui PUBLIC ${imgui_dir})
