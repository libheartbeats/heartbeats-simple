/**
 * Just tests that the functions are all there.
 */
#include <inttypes.h>
#include <stdlib.h>

#include <heartbeats-simple.h>

static const uint64_t window_size = 20;

static void test_hb_container(void) {
  heartbeat_container hc;
  heartbeat_container_init(&hc, window_size);
  heartbeat_container_finish(&hc);
  heartbeat_container_init_context(&hc, window_size, -1, NULL);
  heartbeat_container_finish(&hc);
}

static void test_hb_acc_container(void) {
  heartbeat_acc_container hc;
  heartbeat_acc_container_init(&hc, window_size);
  heartbeat_acc_container_finish(&hc);
  heartbeat_acc_container_init_context(&hc, window_size, -1, NULL);
  heartbeat_acc_container_finish(&hc);
}

static void test_hb_pow_container(void) {
  heartbeat_pow_container hc;
  heartbeat_pow_container_init(&hc, window_size);
  heartbeat_pow_container_finish(&hc);
  heartbeat_pow_container_init_context(&hc, window_size, -1, NULL);
  heartbeat_pow_container_finish(&hc);
}

static void test_hb_acc_pow_container(void) {
  heartbeat_acc_pow_container hc;
  heartbeat_acc_pow_container_init(&hc, window_size);
  heartbeat_acc_pow_container_finish(&hc);
  heartbeat_acc_pow_container_init_context(&hc, window_size, -1, NULL);
  heartbeat_acc_pow_container_finish(&hc);
}

int main(void) {
  test_hb_container();
  test_hb_acc_container();
  test_hb_pow_container();
  test_hb_acc_pow_container();
  return 0;
}
