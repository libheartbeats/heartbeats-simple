/**
 *  Example of a heartbeat.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif
#include <inttypes.h>
#include <unistd.h>
#include "heartbeat-pow.h"

#define UNUSED(x) (void)(x)

// Callback function for when a window is complete (window buffer is full)
void window_complete(const heartbeat_pow_context* hb) {
  // dummy function
  UNUSED(hb);
}

// Simulate energy readings
static inline uint64_t get_energy() {
  static uint64_t energy = 0;
  return energy += 1000;
}

// get time from the system in nanoseconds
static inline uint64_t get_time() {
  struct timespec ts;
#ifdef __MACH__
  // OS X does not have clock_gettime, use clock_get_time
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  ts.tv_sec = mts.tv_sec;
  ts.tv_nsec = mts.tv_nsec;
#else
  clock_gettime(CLOCK_REALTIME, &ts);
#endif
  return ts.tv_sec * 1000000000 + ts.tv_nsec;
}

int main(void) {
  uint64_t i;
  const unsigned int iterations = 10;
  const uint64_t window_size = 5;
  uint64_t start_time, end_time;
  uint64_t start_energy, end_energy;
  int fd = fileno(stdout);

  // Alternatively, a window buffer can be allocated on the stack with a
  // statically sized array - just don't let it go out of scope before
  // the heartbeat!
  heartbeat_pow_record* window_buffer = malloc(window_size * sizeof(heartbeat_pow_record));

  // initialize heartbeat
  heartbeat_pow_context hb;
  heartbeat_pow_init(&hb, window_size, window_buffer, fd, &window_complete);
  hb_pow_log_header(fd);

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
