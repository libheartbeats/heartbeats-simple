cmake_minimum_required(VERSION 2.8.9)

project(heartbeats-simple)
set(PROJECT_VERSION 0.3.2)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")

include_directories(${PROJECT_SOURCE_DIR}/inc)

# force fPIC unless overridden (not automatically forwarded through OBJECT libraries when BUILD_SHARED_LIBS is set)
if (NOT DEFINED CMAKE_POSITION_INDEPENDENT_CODE)
  set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
endif()

# Libraries

add_library(hbs OBJECT src/hb.c src/hb-util.c src/hb-container.c)

add_library(hbs-acc OBJECT src/hb.c src/hb-util.c src/hb-acc-util.c src/hb-container.c)
set_target_properties(hbs-acc PROPERTIES COMPILE_DEFINITIONS "HEARTBEAT_MODE_ACC;HEARTBEAT_USE_ACC")

add_library(hbs-pow OBJECT src/hb.c src/hb-util.c src/hb-pow-util.c src/hb-container.c)
set_target_properties(hbs-pow PROPERTIES COMPILE_DEFINITIONS "HEARTBEAT_MODE_POW;HEARTBEAT_USE_POW")

add_library(hbs-acc-pow OBJECT src/hb.c src/hb-util.c src/hb-acc-util.c src/hb-pow-util.c src/hb-container.c)
set_target_properties(hbs-acc-pow PROPERTIES COMPILE_DEFINITIONS "HEARTBEAT_MODE_ACC_POW;HEARTBEAT_USE_ACC;HEARTBEAT_USE_POW")

add_library(heartbeats-simple $<TARGET_OBJECTS:hbs>
                              $<TARGET_OBJECTS:hbs-acc>
                              $<TARGET_OBJECTS:hbs-pow>
                              $<TARGET_OBJECTS:hbs-acc-pow>)


# Examples

add_executable(hb-pow-example example/hb-pow-example.c)
target_link_libraries(hb-pow-example heartbeats-simple)


# Tests

add_executable(hb-test test/hb-test.c)
target_link_libraries(hb-test heartbeats-simple)

add_executable(hb-container-test test/hb-container-test.c)
target_link_libraries(hb-container-test heartbeats-simple)

enable_testing()
macro(add_unit_test target)
  add_test(${target} ${EXECUTABLE_OUTPUT_PATH}/${target})
endmacro(add_unit_test)

add_unit_test(hb-test)
add_unit_test(hb-container-test)


# pkg-config

set(PKG_CONFIG_EXEC_PREFIX "\${prefix}")
set(PKG_CONFIG_LIBDIR "\${prefix}/lib")
set(PKG_CONFIG_INCLUDEDIR "\${prefix}/include/${PROJECT_NAME}")
set(PKG_CONFIG_CFLAGS "-I\${includedir}")

set(PKG_CONFIG_NAME "heartbeats-simple")
set(PKG_CONFIG_DESCRIPTION "Simple performance monitoring API with optional accuracy and power/energy tracking")
set(PKG_CONFIG_LIBS "-L\${libdir} -lheartbeats-simple")
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/pkgconfig.in
  ${CMAKE_CURRENT_BINARY_DIR}/pkgconfig/heartbeats-simple.pc
)


# Install

install(TARGETS heartbeats-simple DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/inc/ DESTINATION include/${PROJECT_NAME})
install(DIRECTORY ${CMAKE_BINARY_DIR}/pkgconfig/ DESTINATION lib/pkgconfig)


# Uninstall

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
  @ONLY
)

add_custom_target(uninstall COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
