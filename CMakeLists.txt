# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.1)

project(heartbeats-simple VERSION 0.4.0
                          LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
add_compile_options(-Wall)

include(GNUInstallDirs)

enable_testing()
add_subdirectory(test)
add_subdirectory(example)


# Libraries

add_library(hbs OBJECT src/hb.c src/hb-util.c src/hb-container.c)
target_include_directories(hbs PRIVATE ${PROJECT_SOURCE_DIR}/inc)

add_library(hbs-acc OBJECT src/hb.c src/hb-util.c src/hb-acc-util.c src/hb-container.c)
target_include_directories(hbs-acc PRIVATE ${PROJECT_SOURCE_DIR}/inc)
target_compile_definitions(hbs-acc PRIVATE HEARTBEAT_MODE_ACC HEARTBEAT_USE_ACC)

add_library(hbs-pow OBJECT src/hb.c src/hb-util.c src/hb-pow-util.c src/hb-container.c)
target_include_directories(hbs-pow PRIVATE ${PROJECT_SOURCE_DIR}/inc)
target_compile_definitions(hbs-pow PRIVATE HEARTBEAT_MODE_POW HEARTBEAT_USE_POW)

add_library(hbs-acc-pow OBJECT src/hb.c src/hb-util.c src/hb-acc-util.c src/hb-pow-util.c src/hb-container.c)
target_include_directories(hbs-acc-pow PRIVATE ${PROJECT_SOURCE_DIR}/inc)
target_compile_definitions(hbs-acc-pow PRIVATE HEARTBEAT_MODE_ACC_POW HEARTBEAT_USE_ACC HEARTBEAT_USE_POW)

# Some environments require explicit PIC for OBJECT libs used in SHARED libs, e.g., GCC 4.8.5 on CentOS 7
# However, don't override user-specified PIC value, if set
if (BUILD_SHARED_LIBS AND NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
  set_target_properties(hbs hbs-acc hbs-pow hbs-acc-pow PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

add_library(heartbeats-simple $<TARGET_OBJECTS:hbs>
                              $<TARGET_OBJECTS:hbs-acc>
                              $<TARGET_OBJECTS:hbs-pow>
                              $<TARGET_OBJECTS:hbs-acc-pow>)
target_include_directories(heartbeats-simple PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/inc>
                                                    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>)
set(HEARTBEATS_SIMPLE_HEADERS inc/heartbeat-common-types.h
                              inc/heartbeat.h
                              inc/heartbeat-acc.h
                              inc/heartbeat-pow.h
                              inc/heartbeat-acc-pow.h
                              inc/heartbeats-simple.h
                              inc/heartbeat-container.h
                              inc/heartbeat-acc-container.h
                              inc/heartbeat-pow-container.h
                              inc/heartbeat-acc-pow-container.h)
set_target_properties(heartbeats-simple PROPERTIES PUBLIC_HEADER "${HEARTBEATS_SIMPLE_HEADERS}")
if (BUILD_SHARED_LIBS)
  set_target_properties(heartbeats-simple PROPERTIES VERSION ${PROJECT_VERSION}
                                                     SOVERSION ${PROJECT_VERSION_MAJOR})
endif()


# pkg-config

set(PKG_CONFIG_EXEC_PREFIX "\${prefix}")
set(PKG_CONFIG_LIBDIR "\${prefix}/${CMAKE_INSTALL_LIBDIR}")
set(PKG_CONFIG_INCLUDEDIR "\${prefix}/${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}")
set(PKG_CONFIG_CFLAGS "-I\${includedir}")

set(PKG_CONFIG_NAME "heartbeats-simple")
set(PKG_CONFIG_DESCRIPTION "Simple performance monitoring API with optional accuracy and power/energy tracking")
set(PKG_CONFIG_LIBS "-L\${libdir} -lheartbeats-simple")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig.in
  ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/heartbeats-simple.pc
)


# CMake package helper

include(CMakePackageConfigHelpers)

set(HEARTBEATS_SIMPLE_CMAKE_CONFIG_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/HeartbeatsSimple)
set(CONFIG_TARGETS_FILE HeartbeatsSimpleTargets.cmake)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/HeartbeatsSimpleConfig.cmake
  INSTALL_DESTINATION ${HEARTBEATS_SIMPLE_CMAKE_CONFIG_DIR}
)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/HeartbeatsSimpleConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)


# Install

install(TARGETS heartbeats-simple
        EXPORT HeartbeatsSimpleTargets
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/ DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/HeartbeatsSimpleConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/HeartbeatsSimpleConfigVersion.cmake
        DESTINATION ${HEARTBEATS_SIMPLE_CMAKE_CONFIG_DIR})
install(EXPORT HeartbeatsSimpleTargets
        DESTINATION ${HEARTBEATS_SIMPLE_CMAKE_CONFIG_DIR}
        NAMESPACE HeartbeatsSimple::)


# Uninstall

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
  @ONLY
)

add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
