/**
 * A performance monitoring API.
 *
 * @author Connor Imes
 */
#ifndef _HEARTBEAT_H_
#define _HEARTBEAT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "heartbeat-common-types.h"

struct heartbeat_context;

typedef struct heartbeat_record {
  uint64_t id;
  uint64_t user_tag;

  uint64_t work;
  heartbeat_udata wd;
  uint64_t start_time;
  uint64_t end_time;
  heartbeat_udata td;
  heartbeat_rates perf;
} heartbeat_record;

typedef void (heartbeat_window_complete) (const struct heartbeat_context* hb);

typedef struct heartbeat_context {
  heartbeat_window_state ws;
  heartbeat_record* window_buffer;
  uint64_t counter;
  volatile int lock;
  heartbeat_window_complete* hwc_callback;

  // data
  heartbeat_udata td;
  heartbeat_udata wd;
} heartbeat_context;

/**
 * Initialize a heartbeats instance.
 * Sets errno on failure.
 * Only fails if hb is NULL, window_size is 0, or window_buffer is NULL.
 *
 * @param hb
 * @param window_size
 * @param window_buffer
 * @param log_fd
 * @param hwc_callback
 * @return 0 on success, another value otherwise
 */
int heartbeat_init(heartbeat_context* hb,
                   uint64_t window_size,
                   heartbeat_record* window_buffer,
                   int log_fd,
                   heartbeat_window_complete* hwc_callback);

/**
 * Registers a heartbeat.
 *
 * @param hb
 * @param user_tag
 * @param work
 * @param start_time (ns)
 * @param end_time (ns)
 */
void heartbeat(heartbeat_context* hb,
               uint64_t user_tag,
               uint64_t work,
               uint64_t start_time,
               uint64_t end_time);

/**
 * Write the header text to a log file.
 * Sets errno on failure.
 *
 * @param fd
 * @return 0 on success, error code otherwise
 */
int hb_log_header(int fd);

/**
 * Logs the circular window buffer up to the current read index.
 * Sets errno on failure.
 *
 * @param hb
 * @param fd
 * @return 0 on success, error code otherwise
 */
int hb_log_window_buffer(const heartbeat_context* hb, int fd);

/**
 * Returns the size of the sliding window used to compute the current heart
 * rate
 *
 * @param hb pointer to heartbeat_t
 * @return the size of the sliding window (uint64_t)
 */
uint64_t hb_get_window_size(const heartbeat_context* hb);

/**
 * Returns the log file descriptor
 *
 * @param hb pointer to heartbeat_t
 * @return the log file descriptor (int)
 */
int hb_get_log_fd(const heartbeat_context* hb);

/**
 * Returns the current user tag
 *
 * @param hb pointer to heartbeat_t
 * @return the current user tag (uint64_t)
 */
uint64_t hb_get_user_tag(const heartbeat_context* hb);

/**
 * Get the total time for the life of this heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the total time (uint64_t)
 */
uint64_t hb_get_global_time(const heartbeat_context* hb);

/**
 * Get the current window time for this heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the window time (uint64_t)
 */
uint64_t hb_get_window_time(const heartbeat_context* hb);

/**
 * Get the total work for the life of this heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the total work (uint64_t)
 */
uint64_t hb_get_global_work(const heartbeat_context* hb);

/**
 * Get the current window work for this heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the window work (uint64_t)
 */
uint64_t hb_get_window_work(const heartbeat_context* hb);

/**
 * Returns the performance over the life of the entire application
 *
 * @param hb pointer to heartbeat_t
 * @return the performance (double) over the entire life of the application
 */
double hb_get_global_perf(const heartbeat_context* hb);

/**
 * Returns the performance over the last window (as specified to init)
 * heartbeats
 *
 * @param hb pointer to heartbeat_t
 * @return the performance (double) over the last window
 */
double hb_get_window_perf(const heartbeat_context* hb);

/**
 * Returns the performance for the last heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the performance (double) for the last heartbeat
 */
double hb_get_instant_perf(const heartbeat_context* hb);

#ifdef __cplusplus
 }
#endif

#endif
