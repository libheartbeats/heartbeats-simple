/**
 * Heartbeat API type definitions.
 *
 * @author Connor Imes
 */
#ifndef _HEARTBEAT_TYPES_H_
#define _HEARTBEAT_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct heartbeat_context;
typedef struct heartbeat_context heartbeat_context;

typedef struct heartbeat_udata {
  uint64_t global;
  uint64_t window;
} heartbeat_udata;

typedef struct heartbeat_record {
  uint64_t id;
  uint64_t user_tag;

  uint64_t work;
  uint64_t start_time;
  uint64_t end_time;
  double global_perf;
  double window_perf;
  double instant_perf;

  uint64_t start_energy;
  uint64_t end_energy;
  double global_pwr;
  double window_pwr;
  double instant_pwr;
} heartbeat_record;

typedef void (heartbeat_window_complete) (const heartbeat_context* hb,
                                          const heartbeat_record* window_buffer,
                                          uint64_t window_size);

struct heartbeat_context {
  uint64_t counter;
  uint64_t buffer_index;
  uint64_t read_index;
  uint64_t window_size;
  volatile int lock;
  heartbeat_record* window_buffer;
  heartbeat_window_complete* hwc_callback;

  // data
  heartbeat_udata td;
  heartbeat_udata wd;
  heartbeat_udata ed;
};

#ifdef __cplusplus
}
#endif

#endif
