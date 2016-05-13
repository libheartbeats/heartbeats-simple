cmake_minimum_required(VERSION 2.8.9)

project(heartbeats-simple)
set(PROJECT_VERSION 0.2.2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unknown-pragmas -Wextra -pedantic -pedantic-errors -std=gnu99")
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# Build shared objects by default
if(NOT DEFINED BUILD_SHARED_LIBS)
  set(BUILD_SHARED_LIBS TRUE CACHE STRING "Whether to build shared or static libs")
endif()

include_directories(${PROJECT_SOURCE_DIR}/inc)

# force fPIC on static libs
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

# Libraries

add_library(hbs src/hb.c src/hb-util.c src/hb-container.c)

add_library(hbs-acc src/hb.c src/hb-util.c src/hb-acc-util.c src/hb-container.c)
set_target_properties(hbs-acc PROPERTIES COMPILE_DEFINITIONS "HEARTBEAT_MODE_ACC;HEARTBEAT_USE_ACC")

add_library(hbs-pow src/hb.c src/hb-util.c src/hb-pow-util.c src/hb-container.c)
set_target_properties(hbs-pow PROPERTIES COMPILE_DEFINITIONS "HEARTBEAT_MODE_POW;HEARTBEAT_USE_POW")

add_library(hbs-acc-pow src/hb.c src/hb-util.c src/hb-acc-util.c src/hb-pow-util.c src/hb-container.c)
set_target_properties(hbs-acc-pow PROPERTIES COMPILE_DEFINITIONS "HEARTBEAT_MODE_ACC_POW;HEARTBEAT_USE_ACC;HEARTBEAT_USE_POW")

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

add_executable(hb-container-test test/hb-container-test.c)
target_link_libraries(hb-container-test hbs)
add_executable(hb-acc-container-test test/hb-acc-container-test.c)
target_link_libraries(hb-acc-container-test hbs-acc)
add_executable(hb-pow-container-test test/hb-pow-container-test.c)
target_link_libraries(hb-pow-container-test hbs-pow)
add_executable(hb-acc-pow-container-test test/hb-acc-pow-container-test.c)
target_link_libraries(hb-acc-pow-container-test hbs-acc-pow)

enable_testing()
macro(add_unit_test target)
  add_test(${target} ${EXECUTABLE_OUTPUT_PATH}/${target})
endmacro(add_unit_test)

add_unit_test(hb-test)
add_unit_test(hb-acc-test)
add_unit_test(hb-pow-test)
add_unit_test(hb-acc-pow-test)

add_unit_test(hb-container-test)
add_unit_test(hb-acc-container-test)
add_unit_test(hb-pow-container-test)
add_unit_test(hb-acc-pow-container-test)

# pkg-config

set(PKG_CONFIG_EXEC_PREFIX "\${prefix}")
set(PKG_CONFIG_LIBDIR "\${prefix}/lib")
set(PKG_CONFIG_INCLUDEDIR "\${prefix}/include/${PROJECT_NAME}")
set(PKG_CONFIG_CFLAGS "-I\${includedir}")

set(PKG_CONFIG_NAME "heartbeats-simple")
set(PKG_CONFIG_DESCRIPTION "Simple performance monitoring API")
set(PKG_CONFIG_LIBS "-L\${libdir} -lhbs")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig.in
  ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/hbs.pc
)

set(PKG_CONFIG_NAME "heartbeats-simple-acc")
set(PKG_CONFIG_DESCRIPTION "Simple performance monitoring API with accuracy tracking")
set(PKG_CONFIG_LIBS "-L\${libdir} -lhbs-acc")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig.in
  ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/hbs-acc.pc
)

set(PKG_CONFIG_NAME "heartbeats-simple-pow")
set(PKG_CONFIG_DESCRIPTION "Simple performance monitoring API with power/energy tracking")
set(PKG_CONFIG_LIBS "-L\${libdir} -lhbs-pow")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig.in
  ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/hbs-pow.pc
)

set(PKG_CONFIG_NAME "heartbeats-simple-acc-pow")
set(PKG_CONFIG_DESCRIPTION "Simple performance monitoring API with accuracy and power/energy tracking")
set(PKG_CONFIG_LIBS "-L\${libdir} -lhbs-acc-pow")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig.in
  ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/hbs-acc-pow.pc
)

# Install

install(TARGETS hbs hbs-acc hbs-pow hbs-acc-pow DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/inc/ DESTINATION include/${PROJECT_NAME})
install(DIRECTORY ${CMAKE_BINARY_DIR}/pkgconfig/ DESTINATION lib/pkgconfig)

# Uninstall

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
  @ONLY
)

add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
