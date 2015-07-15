/**
 *  Example of a heartbeat.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <unistd.h>
#include "heartbeat-pow.h"

void buffer_full_cb(heartbeat_t* hb,
                    heartbeat_record_t* window_buffer,
                    uint64_t window_size) {
  // we should log the data or else we'll lose it
  heartbeat_log_window_buffer(hb, stdout, 0);
}

static inline int64_t get_time() {
  struct timespec time_info;
  clock_gettime(CLOCK_REALTIME, &time_info);
  return (int64_t) time_info.tv_sec * 1000000000 + (int64_t) time_info.tv_nsec;
}

int main(int argc, char** argv) {
  uint64_t i;
  const int iterations = 20;
  const int window_size = 20;
  int64_t start_time;
  int64_t end_time;

  // initialize heartbeat
  heartbeat_t heart;
  heartbeat_record_t* window_buffer = malloc(window_size * sizeof(heartbeat_record_t));
  heartbeat_init(&heart, window_size, window_buffer, &buffer_full_cb);

  for(i = 0; i < iterations; i++) {
    start_time = get_time();
    usleep(100000);
    end_time = get_time();
    heartbeat(&heart, i, 1, start_time, end_time);
  }

  // cleanup
  free(window_buffer);

  return 0;
}
