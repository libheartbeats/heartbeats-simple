# SPDX-License-Identifier: BSD-3-Clause

macro(add_unit_test target)
  add_test(${target} ${target})
endmacro(add_unit_test)

add_executable(hb-test hb-test.c)
target_link_libraries(hb-test PRIVATE heartbeats-simple)
add_unit_test(hb-test)

add_executable(hb-container-test hb-container-test.c)
target_link_libraries(hb-container-test PRIVATE heartbeats-simple)
add_unit_test(hb-container-test)
