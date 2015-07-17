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

#include "heartbeat-types.h"

/**
 * Initialize a heartbeats instance.
 *
 * @param hb
 * @param window_size
 * @param window_buffer
 * @param hwc_callback
 * @return 0 on success, another value otherwise
 */
int heartbeat_init(heartbeat_context* hb,
                   uint64_t window_size,
                   heartbeat_record* window_buffer,
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
 * Logs the circular window buffer up to the current read index.
 *
 * @param hb
 * @param fd
 * @param print_header
 * @return 0 on success, error code otherwise
 */
int heartbeat_log_window_buffer(const heartbeat_context* hb, int fd, int print_header);

/**
 * Returns the size of the sliding window used to compute the current heart
 * rate
 *
 * @param hb pointer to heartbeat_t
 * @return the size of the sliding window (uint64_t)
 */
uint64_t hb_get_window_size(const heartbeat_context* hb);

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
 * Returns the heart rate over the life of the entire application
 *
 * @param hb pointer to heartbeat_t
 * @return the heart rate (double) over the entire life of the application
 */
double hb_get_global_rate(const heartbeat_context* hb);

/**
 * Returns the heart rate over the last window (as specified to init)
 * heartbeats
 *
 * @param hb pointer to heartbeat_t
 * @return the heart rate (double) over the last window
 */
double hb_get_window_rate(const heartbeat_context* hb);

/**
 * Returns the heart rate for the last heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the heart rate (double) for the last heartbeat
 */
double hb_get_instant_rate(const heartbeat_context* hb);

/**
 * Returns the record for the current heartbeat
 * currently may read old data
 *
 * @param hb pointer to heartbeat_t
 * @param record pointer to record to fill
 */
void hb_get_current(const heartbeat_context* hb,
                    heartbeat_record* record);

/**
 * Returns all heartbeat information for the last n heartbeats
 *
 * @param hb pointer to heartbeat_t
 * @param record pointer to heartbeat_record_t
 * @param n uint64_t
 */
uint64_t hb_get_history(const heartbeat_context* hb,
                        heartbeat_record* record,
                        uint64_t n);

#ifdef __cplusplus
 }
#endif

#endif
