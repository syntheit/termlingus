cmake_minimum_required(VERSION 3.15)
project(termlingus CXX)

include(FetchContent)

FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v3.0.0
)

FetchContent_GetProperties(ftxui)

if(NOT ftxui_POPULATED)
    FetchContent_Populate(ftxui)
    add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

set(CMAKE_CXX_STANDARD 11)
include_directories(include)

add_executable(termlingus src/main.cpp src/ytdl.cpp src/gui/gui.cpp src/gui/thumbnail.cpp src/bmp.cpp src/server.cpp src/client.cpp)

add_executable(home src/home.cpp src/gui/thumbnail.cpp src/bmp.cpp)

target_link_libraries(termlingus
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component # Not needed for this example.
    PRIVATE pthread
)

target_link_libraries(home
    PRIVATE ftxui::screen
    PRIVATE ftxui::dom
    PRIVATE ftxui::component
    PRIVATE pthread
)

get_cmake_property(_variableNames VARIABLES)
list(SORT _variableNames)

foreach(_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()