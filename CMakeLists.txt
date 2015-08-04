cmake_minimum_required(VERSION 3.0)

project(heartbeats-simple VERSION 0.1.0)

set(CMAKE_C_FLAGS "-Wall -Wno-unknown-pragmas")
set(CMAKE_BUILD_TYPE Release)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/inc)

# Libraries

add_library(hbs SHARED src/hb.c src/hb-util.c)
add_library(hbs-acc SHARED src/hb.c src/hb-util.c src/hb-acc-util.c)
set_target_properties (hbs-acc PROPERTIES COMPILE_DEFINITIONS "HEARTBEAT_MODE_ACC;HEARTBEAT_USE_ACC")
add_library(hbs-pow SHARED src/hb.c src/hb-util.c src/hb-pow-util.c)
set_target_properties (hbs-pow PROPERTIES COMPILE_DEFINITIONS "HEARTBEAT_MODE_POW;HEARTBEAT_USE_POW")
add_library(hbs-acc-pow SHARED src/hb.c src/hb-util.c src/hb-acc-util.c src/hb-pow-util.c)
set_target_properties (hbs-acc-pow PROPERTIES COMPILE_DEFINITIONS "HEARTBEAT_MODE_ACC_POW;HEARTBEAT_USE_ACC;HEARTBEAT_USE_POW")

# Examples

add_executable(hb-pow-example example/hb-pow-example.c)
target_link_libraries(hb-pow-example hbs-pow)

# Tests

add_executable(hb-test test/hb-test.c)
target_link_libraries(hb-test hbs)
add_executable(hb-acc-test test/hb-acc-test.c)
target_link_libraries(hb-acc-test hbs-acc)
add_executable(hb-pow-test test/hb-pow-test.c)
target_link_libraries(hb-pow-test hbs-pow)
add_executable(hb-acc-pow-test test/hb-acc-pow-test.c)
target_link_libraries(hb-acc-pow-test hbs-acc-pow)

# pkg-config

set(PKG_CONFIG_EXEC_PREFIX "\${prefix}")
set(PKG_CONFIG_LIBDIR "\${prefix}/lib")
set(PKG_CONFIG_INCLUDEDIR "\${prefix}/include/heartbeats-simple")
set(PKG_CONFIG_CFLAGS "-I\${includedir}")

set(PKG_CONFIG_LIBS "-L\${libdir} -lhbs")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig/hbs.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/hbs.pc"
)

set(PKG_CONFIG_LIBS "-L\${libdir} -lhbs-acc")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig/hbs-acc.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/hbs-acc.pc"
)

set(PKG_CONFIG_LIBS "-L\${libdir} -lhbs-pow")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig/hbs-pow.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/hbs-pow.pc"
)

set(PKG_CONFIG_LIBS "-L\${libdir} -lhbs-acc-pow")
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig/hbs-acc-pow.pc.in"
  "${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/hbs-acc-pow.pc"
)

# Install

install(TARGETS hbs hbs-acc hbs-pow hbs-acc-pow DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/inc/ DESTINATION include/${PROJECT_NAME})
install(DIRECTORY ${CMAKE_BINARY_DIR}/pkgconfig/ DESTINATION lib/pkgconfig)

# Uninstall

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
    IMMEDIATE @ONLY)

add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
