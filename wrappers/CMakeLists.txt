find_package(Python COMPONENTS Development)

file(GLOB PYWRAPPER_SOURCES "*.cpp" "*.h")

# re-run custom wrapper generators
file(GLOB PYWRAPPER_TARGETS "autowrappers/*.json")
foreach(WRAPPER_CFG ${PYWRAPPER_TARGETS})
    get_filename_component(STRIP ${WRAPPER_CFG} NAME_WE)
    string(TOLOWER ${STRIP} STRIP)
    #message("py${STRIP}wrapper.cpp")
    #message("py${STRIP}wrapper.h")
    add_custom_command(
        OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/autowrappers/${STRIP}wrapper.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/autowrappers/${STRIP}wrapper.h"
        COMMAND python3 "${CMAKE_CURRENT_SOURCE_DIR}/wrappergen.py" -o "${CMAKE_CURRENT_SOURCE_DIR}/autowrappers" -i ${WRAPPER_CFG}
        DEPENDS "${WRAPPER_CFG}" "${CMAKE_CURRENT_SOURCE_DIR}/wrappergen.py" "${CMAKE_CURRENT_SOURCE_DIR}/libwrappergen/load.py"
    )
    list(APPEND PYWRAPPER_SOURCES "autowrappers/${STRIP}wrapper.cpp")
    list(APPEND PYWRAPPER_SOURCES "autowrappers/${STRIP}wrapper.h")
endforeach()

add_library(paxpython SHARED ${PYWRAPPER_SOURCES})
target_link_libraries(paxpython PUBLIC Qt6::Widgets Qt6::OpenGLWidgets universe)
target_include_directories(paxpython PRIVATE ${Python_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
