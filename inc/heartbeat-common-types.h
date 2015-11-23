/**
 * Common Heartbeat API type definitions.
 *
 * @author Connor Imes
 */
#ifndef _HEARTBEAT_COMMON_TYPES_H_
#define _HEARTBEAT_COMMON_TYPES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

typedef struct heartbeat_udata {
  uint64_t global;
  uint64_t window;
} heartbeat_udata;

typedef struct heartbeat_rates {
  double global;
  double window;
  double instant;
} heartbeat_rates;

typedef struct heartbeat_window_state {
  uint64_t buffer_index;
  uint64_t read_index;
  uint64_t window_size;
  int log_fd;
} heartbeat_window_state;

#ifdef __cplusplus
}
#endif

#endif
