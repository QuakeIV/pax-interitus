cmake_minimum_required(VERSION 3.5)

project(freespace-aurora VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_GENERATOR Ninja)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.2.4/gcc_64") #we are Qt 6.2.4 for now
find_package(QT NAMES Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)
find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)
#get_target_property(QtCore_location Qt6::Core LOCATION)
#get_filename_component(QtCore_libdir ${QtCore_location} DIRECTORY)
#message("wumbo ${QtCore_libdir}")

file(GLOB PROJECT_SOURCES "*.h" "*.ui")
list(APPEND PROJECT_SOURCES "main.cpp;mainwindow.cpp;systemwindow.cpp;celestialwindow.cpp;systemrenderer.cpp;systemrenderermousebackend.cpp")
message(${PROJECT_SOURCES})
add_subdirectory(components)
add_subdirectory(designer_windows)


qt_add_executable(freespace-aurora
    ${PROJECT_SOURCES}
)

qt_add_resources(freespace-aurora "images"
    PREFIX "/images"
    FILES icon.png)

target_link_libraries(freespace-aurora PRIVATE Qt6::Widgets Qt6::OpenGLWidgets universe)

set_target_properties(freespace-aurora PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

# wrapper annex
# =============================== CMake target - libuniverse ===============================


# Define the sample shared library for which we will create bindings.
set(libuniverse_sources universe.cpp orbittype.cpp solarsystemtype.cpp celestialtype.cpp)
add_library(universe SHARED ${libuniverse_sources})
target_link_libraries(universe PUBLIC Qt6::Widgets Qt6::OpenGLWidgets)
#set_property(TARGET libuniverse PROPERTY PREFIX "")

# Apply relevant include and link flags.
find_package(Python COMPONENTS Development)
#target_include_directories(paxpython PRIVATE ${Python_INCLUDE_DIRS})
#message(${Python_INCLUDE_DIRS})


