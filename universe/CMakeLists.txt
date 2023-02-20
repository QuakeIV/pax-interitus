file(GLOB UNIVERSE_SOURCES "*.cpp" "*.h"
    "components/*.cpp" "components/*.h"
    "spacecraft/*.cpp" "spacecraft/*.h"
)

# =============================== CMake target - libuniverse ===============================

# Define the shared library for the backend logic
# this was originally used for python bindings, and conceptually should probalby be kept
add_library(universe SHARED ${UNIVERSE_SOURCES})
target_link_libraries(universe PUBLIC Qt6::Widgets) #QList, QColor currently (maybe more)
target_include_directories(universe PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(universe PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(universe PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)
