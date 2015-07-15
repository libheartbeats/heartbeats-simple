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

struct heartbeat_t;
typedef struct heartbeat_t heartbeat_t;

typedef struct {
  int64_t total_time;
  int64_t window_time;
} heartbeat_time_data;

typedef struct {
  uint64_t total_work;
  uint64_t window_work;
} heartbeat_work_data;

typedef struct {
  uint64_t id;
  uint64_t user_tag;

  uint64_t work;
  int64_t start_time;
  int64_t end_time;
  double global_perf;
  double window_perf;
  double instant_perf;
} heartbeat_record_t;

typedef void (heartbeat_window_complete) (const struct heartbeat_t* hb,
                                          const heartbeat_record_t* window_buffer,
                                          uint64_t window_size);

struct heartbeat_t {
  uint64_t counter;
  uint64_t buffer_index;
  uint64_t read_index;
  uint64_t window_size;
  heartbeat_record_t* window_buffer;
  heartbeat_window_complete* hwc_callback;

  // data
  heartbeat_time_data td;
  heartbeat_work_data wd;
};

#ifdef __cplusplus
}
#endif

#endif
