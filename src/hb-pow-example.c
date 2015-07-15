/**
 *  Example of a heartbeat.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <unistd.h>
#include "heartbeat-pow.h"

void window_complete(const heartbeat_t* hb,
                     const heartbeat_record_t* window_buffer,
                     uint64_t window_size) {
  static int first = 1;
  // we should log the data or else we'll lose it
  heartbeat_log_window_buffer(hb, fileno(stdout), first);
  first = 0;
}

static inline uint64_t get_energy() {
  static uint64_t energy = 0;
  return energy += 1000;
}

static inline uint64_t get_time() {
  struct timespec time_info;
  clock_gettime(CLOCK_REALTIME, &time_info);
  return (uint64_t) time_info.tv_sec * 1000000000 + (uint64_t) time_info.tv_nsec;
}

int main(int argc, char** argv) {
  uint64_t i;
  const int iterations = 10;
  const int window_size = 5;
  uint64_t start_time, end_time;
  uint64_t start_energy, end_energy;

  // initialize heartbeat
  heartbeat_t heart;
  heartbeat_record_t* window_buffer = malloc(window_size * sizeof(heartbeat_record_t));
  heartbeat_init(&heart, window_size, window_buffer, &window_complete);

  for(i = 0; i < iterations; i++) {
    start_time = get_time();
    start_energy = get_energy();
    usleep(1000);
    end_time = get_time();
    end_energy = get_energy();
    heartbeat_pow(&heart, i, 1, start_time, end_time, start_energy, end_energy);
  }

  // cleanup
  free(window_buffer);

  return 0;
}
