/**
 * Just tests that the functions are all there.
 */
#include <inttypes.h>
#include <stdlib.h>
#include "heartbeat-acc-pow.h"

int main(int argc, char** argv) {
  uint64_t window_size = 20;
  heartbeat_acc_pow_context hb;
  heartbeat_acc_pow_record* window_buffer = malloc(window_size * sizeof(heartbeat_acc_pow_record));
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
  return 0;
}
