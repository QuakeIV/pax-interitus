cmake_minimum_required(VERSION 3.5)

project(freespace-aurora VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

cmake_policy(VERSION 3.12) #shutting up annoying policy warnings, only reason this is here
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "$ENV{HOME}/Qt/6.2.4/gcc_64") #we are Qt 6.2.4 for now
find_package(Qt6 COMPONENTS Widgets OpenGLWidgets REQUIRED)

#TODO: prolly update project sources to be named something else (application core?)
add_subdirectory(universe)
add_subdirectory(python) # cpython/future UI stuff
