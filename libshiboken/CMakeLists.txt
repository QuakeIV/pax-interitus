# this is currently based off of a download of the shiboken 6.2.4 repo
# i was unable to get the entire repo to build, however this library in isolation can compile with relatively minor modifications

project(libshiboken)

set(CMAKE_CXX_STANDARD 17)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/sbkversion.h.in"
               "${CMAKE_CURRENT_BINARY_DIR}/sbkversion.h" @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/embed/signature_bootstrap.py"
               "${CMAKE_CURRENT_BINARY_DIR}/embed/signature_bootstrap.py" @ONLY)

# Variable from enclosing scope.
list(TRANSFORM shiboken_python_files
     PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/../shibokenmodule/files.dir/shibokensupport/"
     OUTPUT_VARIABLE embedded_shiboken_files)

if (QUIET_BUILD)
    set(embedding_option "--quiet")
else()
    set(embedding_option "")
endif()

add_custom_command(
    OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/embed/signature_bootstrap_inc.h"
    OUTPUT  "${CMAKE_CURRENT_BINARY_DIR}/embed/signature_inc.h"
    COMMAND python3 -E
            "${CMAKE_CURRENT_SOURCE_DIR}/embed/embedding_generator.py"
            --cmake-dir "${CMAKE_CURRENT_BINARY_DIR}/embed"
            ${embedding_option}
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/embed/embedding_generator.py"
            "${CMAKE_CURRENT_SOURCE_DIR}/embed/signature_bootstrap.py"
            ${embedded_shiboken_files}
    )

set(libshiboken_SRC
basewrapper.cpp
debugfreehook.cpp
gilstate.cpp
helper.cpp
sbkarrayconverter.cpp
sbkcontainer.cpp
sbkconverter.cpp
sbkenum.cpp
sbkfeature_base.cpp
sbkmodule.cpp
sbkstring.cpp
sbkstaticstrings.cpp
sbktypefactory.cpp
bindingmanager.cpp
threadstatesaver.cpp
shibokenbuffer.cpp
pep384impl.cpp
voidptr.cpp
bufferprocs_py37.cpp

embed/signature_bootstrap_inc.h
embed/signature_inc.h

signature/signature.cpp
signature/signature_globals.cpp
signature/signature_extend.cpp
signature/signature_helper.cpp
)

find_package(Python COMPONENTS Development NumPy)

if (NOT "${NUMPY_INCLUDE_DIR}" STREQUAL "")
    message(STATUS "NUMPY_INCLUDE_DIR: " ${NUMPY_INCLUDE_DIR})
    list(APPEND libshiboken_SRC sbknumpyarrayconverter.cpp)
else()
    message(STATUS "NUMPY not found")
endif()

set(APIEXTRACTOR_EXTRA_INCLUDES ${APIEXTRACTOR_EXTRA_INCLUDES} ${LIBXSLT_INCLUDE_DIR} ${LIBXML2_INCLUDE_DIR})

add_library(shiboken SHARED ${libshiboken_SRC})

target_include_directories(shiboken PUBLIC
  ${Python_INCLUDE_DIRS}
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include/shiboken6>
)

if (NOT "${NUMPY_INCLUDE_DIR}" STREQUAL "")
    target_include_directories(shiboken PRIVATE ${NUMPY_INCLUDE_DIR})
    target_compile_definitions(shiboken PRIVATE -DHAVE_NUMPY
                                           PRIVATE -DNPY_NO_DEPRECATED_API=NPY_1_7_API_VERSION)

endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(PYTHON_WITH_DEBUG)
        target_compile_definitions(shiboken PUBLIC "-DPy_DEBUG")
    endif()
    if (PYTHON_WITH_COUNT_ALLOCS)
        target_compile_definitions(shiboken PUBLIC "-DCOUNT_ALLOCS")
    endif()
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_definitions(shiboken PUBLIC "-DNDEBUG")
endif()

set_target_properties(shiboken PROPERTIES
#                                             VERSION 
                                             SOVERSION 6.2.4)
