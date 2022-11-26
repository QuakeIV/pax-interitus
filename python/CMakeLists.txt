find_package(Python COMPONENTS Development)

add_library(paxpython SHARED paxpython.cpp pysystemrenderer.cpp pysystemrenderermousebackend.cpp pysystemrenderer.h)
target_link_libraries(paxpython PUBLIC Qt6::Widgets Qt6::OpenGLWidgets universe)
target_include_directories(paxpython PRIVATE ${Python_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
