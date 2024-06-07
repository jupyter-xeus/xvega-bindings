############################################################################
# Copyright (c) 2020, QuantStack and xeus-SQLite contributors              #
#                                                                          #
# Distributed under the terms of the BSD 3-Clause License.                 #
#                                                                          #
# The full license is in the file LICENSE, distributed with this software. #
############################################################################
cmake_minimum_required(VERSION 3.8)
project(xvega-bindings)

set(XVEGA_BINDINGS_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Versionning
# ===========

file(STRINGS "${XVEGA_BINDINGS_INCLUDE_DIR}/xvega-bindings/xvega_bindings_config.hpp" xvega-bindings_version_defines
     REGEX "#define XVEGA_BINDINGS_VERSION_(MAJOR|MINOR|PATCH)")
foreach(ver ${xvega-bindings_version_defines})
    if(ver MATCHES "#define XVEGA_BINDINGS_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(XVEGA_BINDINGS_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif()
endforeach()
set(${PROJECT_NAME}_VERSION
    ${XVEGA_BINDINGS_VERSION_MAJOR}.${XVEGA_BINDINGS_VERSION_MINOR}.${XVEGA_BINDINGS_VERSION_PATCH})
message(STATUS "xvega-bindings v${${PROJECT_NAME}_VERSION}")

# Dependencies
# ============

find_package(xvega REQUIRED)

# Build
# =====

set(XVEGA_BINDINGS_HEADERS
    ${XVEGA_BINDINGS_INCLUDE_DIR}/xvega-bindings/utils.hpp
    ${XVEGA_BINDINGS_INCLUDE_DIR}/xvega-bindings/xvega_bindings.hpp
    ${XVEGA_BINDINGS_INCLUDE_DIR}/xvega-bindings/xvega_bindings_config.hpp
)

add_library(xvega-bindings INTERFACE)
target_include_directories(xvega-bindings INTERFACE
    $<BUILD_INTERFACE:${XVEGA_BINDINGS_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(xvega-bindings INTERFACE xvega)

# We do not use non-standard C++
set_target_properties(xvega-bindings PROPERTIES CMAKE_CXX_EXTENSIONS OFF)
target_compile_features(xvega-bindings INTERFACE cxx_std_17)

OPTION(XVEGA_BUILD_TESTS "xvega-bindings test suite" OFF)

if(XVEGA_BUILD_TESTS)
    add_subdirectory(test)
endif()

# Installation
# ============

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

install(TARGETS xvega-bindings
        EXPORT ${PROJECT_NAME}-targets)

# Makes the project importable from the build directory
export(EXPORT ${PROJECT_NAME}-targets
       FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

install(FILES ${XVEGA_BINDINGS_HEADERS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/xvega-bindings)

set(XVEGA_BINDINGS_CMAKECONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE STRING "install path for xvega-bindingsConfig.cmake")

configure_package_config_file(${PROJECT_NAME}Config.cmake.in
                              "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
                              INSTALL_DESTINATION ${XVEGA_BINDINGS_CMAKECONFIG_INSTALL_DIR})

# xvega-bindings is header-only and does not depend on the architecture.
# Remove CMAKE_SIZEOF_VOID_P from xvega-bindingsConfigVersion.cmake so that an xvega-bindingsConfig.cmake
# generated for a 64 bit target can be used for 32 bit targets and vice versa.
set(_XVEGA_BINDINGS_CMAKE_SIZEOF_VOID_P ${CMAKE_SIZEOF_VOID_P})
unset(CMAKE_SIZEOF_VOID_P)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                                 VERSION ${${PROJECT_NAME}_VERSION}
                                 COMPATIBILITY AnyNewerVersion)
set(CMAKE_SIZEOF_VOID_P ${_XVEGA_BINDINGS_CMAKE_SIZEOF_VOID_P})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${XVEGA_BINDINGS_CMAKECONFIG_INSTALL_DIR})
install(EXPORT ${PROJECT_NAME}-targets
        FILE ${PROJECT_NAME}Targets.cmake
        DESTINATION ${XVEGA_BINDINGS_CMAKECONFIG_INSTALL_DIR})

