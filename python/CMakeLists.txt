find_package(Python COMPONENTS Development)

file(GLOB PYWRAPPER_SOURCES "*.cpp" "*.h"
#    "components/*.cpp" "components/*.h"
)

add_library(paxpython SHARED ${PYWRAPPER_SOURCES})
target_link_libraries(paxpython PUBLIC Qt6::Widgets Qt6::OpenGLWidgets universe)
target_include_directories(paxpython PRIVATE ${Python_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
