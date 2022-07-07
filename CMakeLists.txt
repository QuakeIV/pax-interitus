cmake_minimum_required(VERSION 3.5)

project(freespace-aurora VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

cmake_policy(VERSION 3.12) #shutting up policy warnings
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.2.4/gcc_64") #we are Qt 6.2.4 for now
find_package(QT NAMES Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)
find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)
find_package(Python COMPONENTS Development)

#TODO: prolly update project sources to be named something else, at this point its the C++ frontend i think
# this is effectively the frontend library
file(GLOB PROJECT_SOURCES "*.ui")
list(APPEND PROJECT_SOURCES
    main.cpp
    mainwindow.cpp mainwindow.h
    systemrenderer.cpp systemrenderermousebackend.cpp systemrenderer.h
    systemwindow.h
    celestialwindow.h
    techwindow.h
)
set(UNIVERSE_SOURCES
    universe.cpp universe.h
    spacecraft.h
    orbittype.cpp orbittype.h 
    solarsystemtype.cpp solarsystemtype.h
    celestialtype.cpp celestialtype.h
)
add_subdirectory(components)
add_subdirectory(designer_windows)


qt_add_executable(freespace-aurora
    ${PROJECT_SOURCES}
)

# TODO: would be really nice to eventually have custom window icons for each kind of window, or at least to begin with broad categories
qt_add_resources(freespace-aurora "images"
    PREFIX "/images"
    FILES icon.png)

target_link_libraries(freespace-aurora PRIVATE Qt6::Widgets Qt6::OpenGLWidgets universe)

set_target_properties(freespace-aurora PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# =============================== CMake target - libuniverse ===============================

# Define the shared library for the backend logic
# this was originally used for python bindings, and conceptually should probalby be kept
add_library(universe SHARED ${UNIVERSE_SOURCES})
target_link_libraries(universe PUBLIC Qt6::Widgets) #QList, QColor currently (maybe more)


