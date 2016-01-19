/**
 * Unit tests. hb-acc-pow covers hb, hb-acc, and hb-pow due to shared code.
 */
#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include "heartbeat-acc-pow.h"

/**
 * Just tests that the functions are all there.
 */
void test_functions_exist() {
  uint64_t window_size = 20;
  heartbeat_acc_pow_context hb;
  heartbeat_acc_pow_record* window_buffer = malloc(window_size * sizeof(heartbeat_acc_pow_record));
  assert(window_buffer);
  heartbeat_acc_pow_init(&hb, window_size, window_buffer, -1, NULL);
  heartbeat_acc_pow(&hb, 0, 1, 0, 1000000000, 1, 0, 1000000);
  hb_acc_pow_log_header(1);
  hb_acc_pow_log_window_buffer(&hb, 1);

  hb_acc_pow_get_window_size(&hb);
  hb_acc_pow_get_log_fd(&hb);
  hb_acc_pow_get_user_tag(&hb);
  hb_acc_pow_get_global_time(&hb);
  hb_acc_pow_get_window_time(&hb);
  hb_acc_pow_get_global_work(&hb);
  hb_acc_pow_get_window_work(&hb);
  hb_acc_pow_get_global_perf(&hb);
  hb_acc_pow_get_window_perf(&hb);
  hb_acc_pow_get_instant_perf(&hb);
  hb_acc_pow_get_global_accuracy(&hb);
  hb_acc_pow_get_window_accuracy(&hb);
  hb_acc_pow_get_global_accuracy_rate(&hb);
  hb_acc_pow_get_window_accuracy_rate(&hb);
  hb_acc_pow_get_instant_accuracy_rate(&hb);
  hb_acc_pow_get_global_energy(&hb);
  hb_acc_pow_get_window_energy(&hb);
  hb_acc_pow_get_global_power(&hb);
  hb_acc_pow_get_window_power(&hb);
  hb_acc_pow_get_instant_power(&hb);

  free(window_buffer);
}

/**
 * Test issuing two heartbeats and verify that values are updated properly
 */
void test_two_hb() {
  uint64_t window_size = 1; // forces a new window on the second heartbeat
  int log_fd = -1;
  heartbeat_acc_pow_window_complete* cb = NULL;
  heartbeat_acc_pow_context hb;
  heartbeat_acc_pow_record* window_buffer = malloc(window_size * sizeof(heartbeat_acc_pow_record));
  assert(window_buffer);
  assert(heartbeat_acc_pow_init(&hb, window_size, window_buffer, log_fd, cb) == 0);

  // first verify basics
  assert(hb_acc_pow_get_window_size(&hb) == window_size);
  assert(hb_acc_pow_get_log_fd(&hb) == log_fd);

  // issue one heartbeat
  uint64_t tag = 10;
  uint64_t work = 1;
  uint64_t start_time = 0;
  uint64_t end_time = 1000000000;
  uint64_t accuracy = 1;
  uint64_t start_energy = 0;
  uint64_t end_energy = 1000000;

  // first heartbeat
  heartbeat_acc_pow(&hb, tag, work, start_time, end_time, accuracy, start_energy, end_energy);

  // verify data
  assert(hb_acc_pow_get_user_tag(&hb) == tag);
  assert(hb_acc_pow_get_global_time(&hb) == end_time);
  assert(hb_acc_pow_get_window_time(&hb) == end_time);
  assert(hb_acc_pow_get_global_work(&hb) == work);
  assert(hb_acc_pow_get_window_work(&hb) == work);
  assert(hb_acc_pow_get_global_perf(&hb) == 1.0);
  assert(hb_acc_pow_get_window_perf(&hb) == 1.0);
  assert(hb_acc_pow_get_instant_perf(&hb) == 1.0);
  assert(hb_acc_pow_get_global_accuracy(&hb) == accuracy);
  assert(hb_acc_pow_get_window_accuracy(&hb) == accuracy);
  assert(hb_acc_pow_get_global_accuracy_rate(&hb) == accuracy);
  assert(hb_acc_pow_get_window_accuracy_rate(&hb) == accuracy);
  assert(hb_acc_pow_get_instant_accuracy_rate(&hb) == accuracy);
  assert(hb_acc_pow_get_global_energy(&hb) == end_energy);
  assert(hb_acc_pow_get_window_energy(&hb) == end_energy);
  assert(hb_acc_pow_get_global_power(&hb) == 1.0);
  assert(hb_acc_pow_get_window_power(&hb) == 1.0);
  assert(hb_acc_pow_get_instant_power(&hb) == 1.0);

  // second heartbeat
  tag++;
  start_time = end_time;
  end_time *= 2;
  start_energy = end_energy;
  end_energy *= 2;
  heartbeat_acc_pow(&hb, tag, work, start_time, end_time, accuracy, start_energy, end_energy);

  // verify data
  assert(hb_acc_pow_get_user_tag(&hb) == tag);
  assert(hb_acc_pow_get_global_time(&hb) == end_time);
  assert(hb_acc_pow_get_window_time(&hb) == end_time - start_time);
  assert(hb_acc_pow_get_global_work(&hb) == 2 * work);
  assert(hb_acc_pow_get_window_work(&hb) == work);
  assert(hb_acc_pow_get_global_perf(&hb) == 1.0);
  assert(hb_acc_pow_get_window_perf(&hb) == 1.0);
  assert(hb_acc_pow_get_instant_perf(&hb) == 1.0);
  assert(hb_acc_pow_get_global_accuracy(&hb) == 2 * accuracy);
  assert(hb_acc_pow_get_window_accuracy(&hb) == accuracy);
  assert(hb_acc_pow_get_global_accuracy_rate(&hb) == accuracy);
  assert(hb_acc_pow_get_window_accuracy_rate(&hb) == accuracy);
  assert(hb_acc_pow_get_instant_accuracy_rate(&hb) == accuracy);
  assert(hb_acc_pow_get_global_energy(&hb) == end_energy);
  assert(hb_acc_pow_get_window_energy(&hb) == end_energy - start_energy);
  assert(hb_acc_pow_get_global_power(&hb) == 1.0);
  assert(hb_acc_pow_get_window_power(&hb) == 1.0);
  assert(hb_acc_pow_get_instant_power(&hb) == 1.0);

  free(window_buffer);
}

int received_cb = 0;
void callback(const heartbeat_acc_pow_context* hb) {
  if (hb != NULL) {
    received_cb++;
  }
}

/**
 * Test that a specified callback function is executed on window completion
 */
void test_callback() {
  uint64_t window_size = 1;
  int log_fd = -1;
  heartbeat_acc_pow_window_complete* cb = &callback;
  heartbeat_acc_pow_context hb;
  heartbeat_acc_pow_record* window_buffer = malloc(window_size * sizeof(heartbeat_acc_pow_record));
  assert(window_buffer);
  assert(heartbeat_acc_pow_init(&hb, window_size, window_buffer, log_fd, cb) == 0);
  heartbeat_acc_pow(&hb, 0, 1, 0, 1000000000, 0, 0, 0);
  assert(received_cb);

  free(window_buffer);
}

/**
 * Test that bad arguments don't crash and return proper error codes
 */
void test_bad_arguments() {
  uint64_t window_size = 20;
  heartbeat_acc_pow_context hb;
  heartbeat_acc_pow_record* window_buffer = malloc(window_size * sizeof(heartbeat_acc_pow_record));
  assert(window_buffer);

  // start with a normal init
  assert(heartbeat_acc_pow_init(&hb, window_size, window_buffer, -1, NULL) == 0);

  // test zero values
  heartbeat_acc_pow(&hb, 0, 0, 0, 0, 0, 0, 0);

  // issue a real heartbeat now
  heartbeat_acc_pow(&hb, 0, 1, 0, 1000000000, 1, 0, 1000000);

  // NULL heartbeats
  assert(heartbeat_acc_pow_init(NULL, window_size, window_buffer, -1, NULL));
  heartbeat_acc_pow(NULL, 0, 1, 0, 1000000000, 1, 0, 1000000);
  assert(hb_acc_pow_log_window_buffer(NULL, 1));
  assert(hb_acc_pow_get_window_size(NULL) == 0);
  assert(hb_acc_pow_get_log_fd(NULL) <= 0);
  assert(hb_acc_pow_get_user_tag(NULL) == 0);
  assert(hb_acc_pow_get_global_time(NULL) == 0);
  assert(hb_acc_pow_get_window_time(NULL) == 0);
  assert(hb_acc_pow_get_global_work(NULL) == 0);
  assert(hb_acc_pow_get_window_work(NULL) == 0);
  assert(hb_acc_pow_get_global_perf(NULL) == 0);
  assert(hb_acc_pow_get_window_perf(NULL) == 0);
  assert(hb_acc_pow_get_instant_perf(NULL) == 0);
  assert(hb_acc_pow_get_global_accuracy(NULL) == 0);
  assert(hb_acc_pow_get_window_accuracy(NULL) == 0);
  assert(hb_acc_pow_get_global_accuracy_rate(NULL) == 0);
  assert(hb_acc_pow_get_window_accuracy_rate(NULL) == 0);
  assert(hb_acc_pow_get_instant_accuracy_rate(NULL) == 0);
  assert(hb_acc_pow_get_global_energy(NULL) == 0);
  assert(hb_acc_pow_get_window_energy(NULL) == 0);
  assert(hb_acc_pow_get_global_power(NULL) == 0);
  assert(hb_acc_pow_get_window_power(NULL) == 0);
  assert(hb_acc_pow_get_instant_power(NULL) == 0);

  // NULL window buffer
  assert(heartbeat_acc_pow_init(&hb, window_size, NULL, -1, NULL));
  // 0 window size
  assert(heartbeat_acc_pow_init(&hb, 0, window_buffer, -1, NULL));

  // bad file descriptors
  assert(hb_acc_pow_log_window_buffer(&hb, -1));
  assert(hb_acc_pow_log_header(-1));

  free(window_buffer);
}

int main(void) {
  test_functions_exist();
  test_two_hb();
  test_callback();
  test_bad_arguments();
  return 0;
}
