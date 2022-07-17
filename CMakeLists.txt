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

#TODO: prolly update project sources to be named something else (application core?)
file(GLOB PROJECT_SOURCES "*.ui" "*.cpp" "*.h")
add_subdirectory(universe)
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


