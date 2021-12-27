cmake_minimum_required(VERSION 3.5)

project(freespace-aurora VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "/home/matthew/Qt/6.2.2/gcc_64;/Users/MatthewGlitsch/Qt/6.2.2/macos") 
find_package(QT NAMES Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)
find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp mainwindow.h mainwindow.ui
        systemrenderer.h systemrenderer.cpp systemrenderermousebackend.cpp
        universe.h universe.cpp
        solarsystemtype.h solarsystemtype.cpp
        celestialtype.h celestialtype.cpp
        transform.h transform.cpp
        orbittype.h orbittype.cpp
        fleettype.h fleettype.cpp
        fixedv2d.h
        celestialwindow.h celestialwindow.cpp celestialwindow.ui
)


qt_add_executable(freespace-aurora
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

qt_add_resources(freespace-aurora "images"
    PREFIX "/images"
    FILES icon.png)

target_link_libraries(freespace-aurora PRIVATE Qt6::Widgets Qt6::OpenGLWidgets)

set_target_properties(freespace-aurora PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

qt_finalize_executable(freespace-aurora)
