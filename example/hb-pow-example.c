/**
 *  Example of a heartbeat.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <unistd.h>
#include "heartbeat-pow.h"

// Callback function for when a window is complete (window buffer is full)
void window_complete(const heartbeat_pow_context* hb) {
  static int first = 1;
  // we should log the data or else we'll lose it
  heartbeat_pow_log_window_buffer(hb, fileno(stdout), first);
  first = 0;
}

// Simulate energy readings
static inline uint64_t get_energy() {
  static uint64_t energy = 0;
  return energy += 1000;
}

// get time from the system in nanoseconds
static inline uint64_t get_time() {
  struct timespec time_info;
  clock_gettime(CLOCK_REALTIME, &time_info);
  return time_info.tv_sec * 1000000000 + time_info.tv_nsec;
}

int main(int argc, char** argv) {
  uint64_t i;
  const int iterations = 10;
  const int window_size = 5;
  uint64_t start_time, end_time;
  uint64_t start_energy, end_energy;

  // Alternatively, a window buffer can be allocated on the stack with a
  // statically sized array - just don't let it go out of scope before
  // the heartbeat!
  heartbeat_pow_record* window_buffer = malloc(window_size * sizeof(heartbeat_pow_record));

  // initialize heartbeat
  heartbeat_pow_context hb;
  heartbeat_pow_init(&hb, window_size, window_buffer, &window_complete);

  // simple example - doesn't track heartbeat time/energy overhead
  for(i = 0; i < iterations; i++) {
    start_time = get_time();
    start_energy = get_energy();
    usleep(1000);
    end_time = get_time();
    end_energy = get_energy();
    heartbeat_pow(&hb, i, 1, start_time, end_time, start_energy, end_energy);
  }

  // this loop includes heartbeat time/energy overhead in the heartbeat
  end_time = get_time();
  end_energy = get_energy();
  for(i = 0; i < iterations; i++) {
    start_time = end_time;
    start_energy = end_energy;
    usleep(1000);
    end_time = get_time();
    end_energy = get_energy();
    heartbeat_pow(&hb, i, 1, start_time, end_time, start_energy, end_energy);
  }

  // cleanup memory
  free(window_buffer);

  return 0;
}
