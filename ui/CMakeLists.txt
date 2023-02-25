find_package(Python COMPONENTS Development)

file(GLOB PYWRAPPER_SOURCES "*.ui")

# (re)generate python UI classes automatically
file(GLOB PYUIC_TARGETS "*.ui")
foreach(UIC_TARGET ${PYUIC_TARGETS})
    get_filename_component(STRIP ${UIC_TARGET} NAME_WE)
    #message("${STRIP}.py")
    list(APPEND PYWRAPPER_SOURCES "${CMAKE_BINARY_DIR}/ui/${STRIP}.py")
    add_custom_command(
        OUTPUT "${CMAKE_BINARY_DIR}/ui/${STRIP}.py"
        COMMAND pyside6-uic ${UIC_TARGET} -o "${CMAKE_BINARY_DIR}/ui/${STRIP}.py"
        DEPENDS ${UIC_TARGET}
    )
endforeach()

# add module script to import UI submodules for when you import build.ui
# TODO: really felt like this shouldnt of been needed
add_Custom_command(
    OUTPUT "${CMAKE_BINARY_DIR}/ui/__init__.py"
    COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/__init__.py" "${CMAKE_BINARY_DIR}/ui/__init__.py"
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/__init__.py"
)
list(APPEND PYWRAPPER_SOURCES "${CMAKE_BINARY_DIR}/ui/__init__.py")

add_custom_target(ui ALL SOURCES ${PYWRAPPER_SOURCES})
