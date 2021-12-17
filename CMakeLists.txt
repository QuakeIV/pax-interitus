# Set the name and the supported language of the project
project(aurora-freespace C)
# Set the minimum version of cmake required to build this project
cmake_minimum_required(VERSION 3.0)

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)

#opengl
set(OpenGL_GL_PREFERENCE "GLVND") #express preference for non-legacy opengl
find_package(OpenGL REQUIRED)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
# Add other flags to the compiler
add_definitions(${GTK3_CFLAGS_OTHER})
# Add an executable compiled from hello.c
add_executable(aurora_freespace main.c)
# Link the target to the GTK+ libraries
target_link_libraries(aurora_freespace
${GTK3_LIBRARIES}
GL
GLU
glut
)

add_executable(FUCK g.c)
target_link_libraries(FUCK
${GTK3_LIBRARIES}
GL
GLU
glut
m
)

