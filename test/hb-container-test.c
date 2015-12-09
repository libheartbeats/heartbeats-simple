/**
 * Just tests that the functions are all there.
 */
#include <inttypes.h>
#include <stdlib.h>
#include "heartbeat-container.h"

int main(int argc, char** argv) {
  uint64_t window_size = 20;
  heartbeat_container hc;

  heartbeat_container_init(&hc, window_size);
  heartbeat_container_finish(&hc);

  heartbeat_container_init_context(&hc, window_size, -1, NULL);
  heartbeat_container_finish(&hc);
  return 0;
}
