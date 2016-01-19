/**
 * Just tests that the functions are all there.
 */
#include <inttypes.h>
#include <stdlib.h>
#include "heartbeat-pow-container.h"

int main(void) {
  uint64_t window_size = 20;
  heartbeat_pow_container hc;

  heartbeat_pow_container_init(&hc, window_size);
  heartbeat_pow_container_finish(&hc);

  heartbeat_pow_container_init_context(&hc, window_size, -1, NULL);
  heartbeat_pow_container_finish(&hc);
  return 0;
}
