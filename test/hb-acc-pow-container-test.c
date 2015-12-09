/**
 * Just tests that the functions are all there.
 */
#include <inttypes.h>
#include <stdlib.h>
#include "heartbeat-acc-pow-container.h"

int main(int argc, char** argv) {
  uint64_t window_size = 20;
  heartbeat_acc_pow_container hc;

  heartbeat_acc_pow_container_init(&hc, window_size);
  heartbeat_acc_pow_container_finish(&hc);

  heartbeat_acc_pow_container_init_context(&hc, window_size, -1, NULL);
  heartbeat_acc_pow_container_finish(&hc);
  return 0;
}
