# SPDX-License-Identifier: BSD-3-Clause

add_executable(hb-pow-example hb-pow-example.c)
target_link_libraries(hb-pow-example PRIVATE heartbeats-simple)
