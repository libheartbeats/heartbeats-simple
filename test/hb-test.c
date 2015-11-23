/**
 * Just tests that the functions are all there.
 */
#include <inttypes.h>
#include <stdlib.h>
#include "heartbeat.h"

int main(int argc, char** argv) {
  uint64_t window_size = 20;
  heartbeat_context hb;
  heartbeat_record* window_buffer = malloc(window_size * sizeof(heartbeat_record));
  heartbeat_init(&hb, window_size, window_buffer, -1, NULL);
  heartbeat(&hb, 0, 1, 0, 1000000000);
  hb_log_header(1);
  hb_log_window_buffer(&hb, 1);

  hb_get_window_size(&hb);
  hb_get_log_fd(&hb);
  hb_get_user_tag(&hb);
  hb_get_global_time(&hb);
  hb_get_window_time(&hb);
  hb_get_global_work(&hb);
  hb_get_window_work(&hb);
  hb_get_global_perf(&hb);
  hb_get_window_perf(&hb);
  hb_get_instant_perf(&hb);

  free(window_buffer);
  return 0;
}
