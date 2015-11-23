/**
 * An extension of the capabilities in "heartbeat.h" to include monitoring
 * application accuracy.
 *
 * @author Connor Imes
 */
#ifndef _HEARTBEAT_ACC_H_
#define _HEARTBEAT_ACC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "heartbeat-common-types.h"

struct heartbeat_acc_context;

typedef struct heartbeat_acc_record {
  uint64_t id;
  uint64_t user_tag;

  uint64_t work;
  heartbeat_udata wd;
  uint64_t start_time;
  uint64_t end_time;
  heartbeat_udata td;
  heartbeat_rates perf;

  uint64_t accuracy;
  heartbeat_udata ad;
  heartbeat_rates acc;
} heartbeat_acc_record;

typedef void (heartbeat_acc_window_complete) (const struct heartbeat_acc_context* hb);

typedef struct heartbeat_acc_context {
  heartbeat_window_state ws;
  heartbeat_acc_record* window_buffer;
  uint64_t counter;
  volatile int lock;
  heartbeat_acc_window_complete* hwc_callback;

  // data
  heartbeat_udata td;
  heartbeat_udata wd;
  heartbeat_udata ad;
} heartbeat_acc_context;

/**
 * Initialize a heartbeats instance.
 *
 * @param hb
 * @param window_size
 * @param window_buffer
 * @param log_fd
 * @param hwc_callback
 * @return 0 on success, another value otherwise
 */
int heartbeat_acc_init(heartbeat_acc_context* hb,
                       uint64_t window_size,
                       heartbeat_acc_record* window_buffer,
                       int log_fd,
                       heartbeat_acc_window_complete* hwc_callback);

/**
 * Registers a heartbeat.
 *
 * @param hb
 * @param user_tag
 * @param work
 * @param start_time (ns)
 * @param end_time (ns)
 * @param start_accuracy
 * @param end_accuracy
 */
void heartbeat_acc(heartbeat_acc_context* hb,
                   uint64_t user_tag,
                   uint64_t work,
                   uint64_t start_time,
                   uint64_t end_time,
                   uint64_t accuracy);

/**
 * Write the header text to a log file.
 *
 * @param fd
 * @return 0 on success, error code otherwise
 */
int hb_acc_log_header(int fd);

/**
 * Logs the circular window buffer up to the current read index.
 *
 * @param hb
 * @param fd
 * @return 0 on success, error code otherwise
 */
int hb_acc_log_window_buffer(const heartbeat_acc_context* hb, int fd);

/**
 * Returns the size of the sliding window used to compute the current heart
 * rate
 *
 * @param hb pointer to heartbeat_t
 * @return the size of the sliding window (uint64_t)
 */
uint64_t hb_acc_get_window_size(const heartbeat_acc_context* hb);

/**
 * Returns the log file descriptor
 *
 * @param hb pointer to heartbeat_t
 * @return the log file descriptor (int)
 */
int hb_acc_get_log_fd(const heartbeat_acc_context* hb);

/**
 * Returns the current user tag
 *
 * @param hb pointer to heartbeat_t
 * @return the current user tag (uint64_t)
 */
uint64_t hb_acc_get_user_tag(const heartbeat_acc_context* hb);

/**
 * Get the total time for the life of this heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the total time (uint64_t)
 */
uint64_t hb_acc_get_global_time(const heartbeat_acc_context* hb);

/**
 * Get the current window time for this heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the window time (uint64_t)
 */
uint64_t hb_acc_get_window_time(const heartbeat_acc_context* hb);

/**
 * Get the total work for the life of this heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the total work (uint64_t)
 */
uint64_t hb_acc_get_global_work(const heartbeat_acc_context* hb);

/**
 * Get the current window work for this heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the window work (uint64_t)
 */
uint64_t hb_acc_get_window_work(const heartbeat_acc_context* hb);

/**
 * Returns the performance over the life of the entire application
 *
 * @param hb pointer to heartbeat_t
 * @return the performance (double) over the entire life of the application
 */
double hb_acc_get_global_perf(const heartbeat_acc_context* hb);

/**
 * Returns the performance over the last window (as specified to init)
 * heartbeats
 *
 * @param hb pointer to heartbeat_t
 * @return the performance (double) over the last window
 */
double hb_acc_get_window_perf(const heartbeat_acc_context* hb);

/**
 * Returns the performance for the last heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the performance (double) for the last heartbeat
 */
double hb_acc_get_instant_perf(const heartbeat_acc_context* hb);

/**
 * Get the total accuracy for the life of this heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the total accuracy (uint64_t)
 */
uint64_t hb_acc_get_global_accuracy(const heartbeat_acc_context* hb);

/**
 * Get the current window accuracy for this heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the window accuracy (uint64_t)
 */
uint64_t hb_acc_get_window_accuracy(const heartbeat_acc_context* hb);

/**
 * Returns the accuracy rate over the life of the entire application
 *
 * @param hb pointer to heartbeat_t
 * @return the accuracy (double) over the entire life of the application
 */
double hb_acc_get_global_accuracy_rate(const heartbeat_acc_context* hb);

/**
 * Returns the accuracy rate over the last window (as specified to init)
 * heartbeats
 *
 * @param hb pointer to heartbeat_t
 * @return the accuracy (double) over the last window
 */
double hb_acc_get_window_accuracy_rate(const heartbeat_acc_context* hb);

/**
 * Returns the accuracy rate for the last heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the accuracy (double) for the last heartbeat
 */
double hb_acc_get_instant_accuracy_rate(const heartbeat_acc_context* hb);

#ifdef __cplusplus
}
#endif

#endif
