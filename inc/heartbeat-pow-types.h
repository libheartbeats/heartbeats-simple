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

#include <stdio.h>
#include <stdint.h>

typedef struct {
  int64_t total_time;
  int64_t window_time;
} heartbeat_time_data;

typedef struct {
  uint64_t total_work;
  uint64_t window_work;
} heartbeat_work_data;

typedef struct {
  uint64_t total_energy;
  uint64_t window_energy;
} heartbeat_energy_data;

typedef struct {
  uint64_t id;
  uint64_t user_tag;

  uint64_t work;
  int64_t start_time;
  int64_t end_time;
  double global_perf;
  double window_perf;
  double instant_perf;

  uint64_t energy;
  double global_pwr;
  double window_pwr;
  double instant_pwr;
} heartbeat_record_t;

typedef void (heartbeat_buffer_full) (heartbeat_record_t* window_buffer, uint64_t window_size);

typedef struct {
  uint64_t window_size;
  uint64_t counter;

  // data
  heartbeat_time_data td;
  heartbeat_work_data wd;
  heartbeat_energy_data ed;

  heartbeat_record_t* window_buffer;
  uint64_t buffer_index;
  uint64_t read_index;
  heartbeat_buffer_full* hbf_callback;
} heartbeat_t;

#ifdef __cplusplus
}
#endif

#endif
