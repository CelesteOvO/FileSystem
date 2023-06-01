cmake_minimum_required(VERSION 3.25)
project(FileSystem)

set(SRC
        FileSystem.h
        FileSystem.cpp
        FileSystemInterface.cpp
        FileSystemInterface.h
        Terminal.h
        Terminal.cpp

        deps/imgui/imgui.h
        deps/imgui/imconfig.h
        deps/imgui/imgui_internal.h
        deps/imgui/imstb_rectpack.h
        deps/imgui/imstb_textedit.h
        deps/imgui/imstb_truetype.h
        deps/imgui/implot.h
        deps/imgui/implot_internal.h
        deps/imgui/GraphEditor.h
        deps/imgui/ImCurveEdit.h
        deps/imgui/ImGradient.h
        deps/imgui/ImGuizmo.h
        deps/imgui/ImSequencer.h
        deps/imgui/ImZoomSlider.h
        deps/imgui/backends/imgui_impl_opengl3.h
        deps/imgui/backends/imgui_impl_glfw.h

        deps/imgui/imgui.cpp
        deps/imgui/imgui_draw.cpp
        deps/imgui/imgui_widgets.cpp
        deps/imgui/imgui_tables.cpp
        deps/imgui/implot.cpp
        deps/imgui/implot_items.cpp
        deps/imgui/GraphEditor.cpp
        deps/imgui/ImCurveEdit.cpp
        deps/imgui/ImGradient.cpp
        deps/imgui/ImGuizmo.cpp
        deps/imgui/ImSequencer.cpp
        deps/imgui/backends/imgui_impl_opengl3.cpp
        deps/imgui/backends/imgui_impl_glfw.cpp
        )

if (NOT TARGET glad)
    add_subdirectory(deps/glad)
    target_include_directories(glad PUBLIC deps)
endif ()
if (NOT TARGET glfw)
    add_subdirectory(deps/glfw3)
endif ()

add_library(FileSystem STATIC)
target_sources(FileSystem PUBLIC ${SRC})

set_target_properties(FileSystem PROPERTIES CXX_STANDARD 17 CXX_EXTENSIONS ON)
target_include_directories(FileSystem PUBLIC deps deps/glfw3/include deps/imgui)
target_link_libraries(FileSystem PUBLIC glad)
target_link_libraries(FileSystem PUBLIC glfw)

add_executable(FileSystem_APP main.cpp)

set_target_properties(FileSystem_APP PROPERTIES CXX_STANDARD 20 CXX_EXTENSIONS ON)
target_link_libraries(FileSystem_APP PUBLIC FileSystem)
