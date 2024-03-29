cmake_minimum_required(VERSION 3.5)

project(freespace-aurora VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

cmake_policy(VERSION 3.12) #shutting up annoying policy warnings, only reason this is here
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.2.4/gcc_64") #we are Qt 6.2.4 for now
find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(universe)
add_subdirectory(ui)
add_subdirectory(wrappers) # cpython wrappers to allow the python UI to interact with the universe
