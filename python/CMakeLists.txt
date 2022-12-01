find_package(Python COMPONENTS Development)

file(GLOB PYWRAPPER_SOURCES "*.cpp" "*.h" "*.ui")

# (re)generate python UI classes automatically
file(GLOB PYUIC_TARGETS "*.ui")
foreach(UIC_TARGET ${PYUIC_TARGETS})
    get_filename_component(STRIP ${UIC_TARGET} NAME_WE)
    message("ui_${STRIP}.py")
    list(APPEND PYWRAPPER_SOURCES "${CMAKE_BINARY_DIR}/python/ui_${STRIP}.py")
    add_custom_command(
        OUTPUT "${CMAKE_BINARY_DIR}/python/ui_${STRIP}.py"
        COMMAND pyside6-uic ${UIC_TARGET} -o "${CMAKE_BINARY_DIR}/python/ui_${STRIP}.py"
        DEPENDS ${UIC_TARGET}
    )
endforeach()

add_library(paxpython SHARED ${PYWRAPPER_SOURCES})
target_link_libraries(paxpython PUBLIC Qt6::Widgets Qt6::OpenGLWidgets universe)
target_include_directories(paxpython PRIVATE ${Python_INCLUDE_DIRS} ${PROJECT_SOURCE_DIR})
