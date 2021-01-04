cmake_minimum_required(VERSION 3.1)

project(xvega-bindings VERSION 0.0.3 LANGUAGES CXX)

include(GNUInstallDirs)

set(XV_BINDINGS_TARGET_NAME ${PROJECT_NAME})
set(XV_BINDINGS_INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
set(XV_BINDINGS_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set(XV_BINDINGS_CONFIG_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE INTERNAL "")
set(XV_BINDINGS_TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")
set(XV_BINDINGS_CMAKE_CONFIG_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(XV_BINDINGS_CMAKE_PROJECT_CONFIG_FILE "${XV_BINDINGS_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake")
set(XV_BINDINGS_CMAKE_VERSION_CONFIG_FILE "${XV_BINDINGS_CMAKE_CONFIG_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(XV_BINDINGS_CMAKE_PROJECT_TARGETS_FILE "${XV_BINDINGS_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Targets.cmake")

# Dependencies
# ============
find_package(xvega REQUIRED)

# Targeting
# =========
add_library(${XV_BINDINGS_TARGET_NAME} INTERFACE)
if (${CMAKE_VERSION} VERSION_LESS "3.8.0")
    target_compile_features(${XV_BINDINGS_TARGET_NAME} INTERFACE cxx_range_for)
else()
    target_compile_features(${XV_BINDINGS_TARGET_NAME} INTERFACE cxx_std_11)
endif()

target_include_directories(
    ${XV_BINDINGS_TARGET_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${XV_BINDINGS_INCLUDE_INSTALL_DIR}>
    $<INSTALL_INTERFACE:include>
    PUBLIC
    ${xvega_INCLUDE_DIRS}
)

# Installing
# ==========
include(CMakePackageConfigHelpers)
# use a custom package version config file instead of
# write_basic_package_version_file to ensure that it's architecture-independent
# https://github.com/nlohmann/json/issues/1697
configure_file(
    "cmake/xvega-bindingsConfigVersion.cmake.in"
    ${XV_BINDINGS_CMAKE_VERSION_CONFIG_FILE}
    @ONLY
)
configure_file(
    "cmake/xvega-bindingsConfig.cmake.in"
    ${XV_BINDINGS_CMAKE_PROJECT_CONFIG_FILE}
    @ONLY
)

message(STATUS "include install dir is: ${CMAKE_INSTALL_INCLUDEDIR}")
install(
    DIRECTORY ${XV_BINDINGS_INCLUDE_DIR}
    DESTINATION ${XV_BINDINGS_INCLUDE_INSTALL_DIR}
)
install(
    FILES ${XV_BINDINGS_CMAKE_PROJECT_CONFIG_FILE} ${XV_BINDINGS_CMAKE_VERSION_CONFIG_FILE}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${XV_BINDINGS_CONFIG_INSTALL_DIR}
)
export(
    TARGETS ${XV_BINDINGS_TARGET_NAME}
    FILE ${XV_BINDINGS_CMAKE_PROJECT_TARGETS_FILE}
)
install(
    TARGETS ${XV_BINDINGS_TARGET_NAME}
    EXPORT ${XV_BINDINGS_TARGETS_EXPORT_NAME}
    INCLUDES DESTINATION ${XV_BINDINGS_INCLUDE_INSTALL_DIR}
)
install(
    EXPORT ${XV_BINDINGS_TARGETS_EXPORT_NAME}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/${XV_BINDINGS_CONFIG_INSTALL_DIR}
)
