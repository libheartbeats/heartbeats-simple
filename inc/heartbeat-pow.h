/**
 * An extension of the capabilities in "heartbeat.h" to include monitoring
 * system power/energy usage.
 *
 * @author Connor Imes
 */
#ifndef _HEARTBEAT_POW_H_
#define _HEARTBEAT_POW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "heartbeat-common-types.h"

struct heartbeat_pow_context;

typedef struct heartbeat_pow_record {
  uint64_t id;
  uint64_t user_tag;

  uint64_t work;
  heartbeat_udata wd;
  uint64_t start_time;
  uint64_t end_time;
  heartbeat_udata td;
  heartbeat_rates perf;

  uint64_t start_energy;
  uint64_t end_energy;
  heartbeat_udata ed;
  heartbeat_rates pwr;
} heartbeat_pow_record;

typedef void (heartbeat_pow_window_complete) (const struct heartbeat_pow_context* hb);

typedef struct heartbeat_pow_context {
  heartbeat_window_state ws;
  heartbeat_pow_record* window_buffer;
  uint64_t counter;
  volatile int lock;
  heartbeat_pow_window_complete* hwc_callback;

  // data
  heartbeat_udata td;
  heartbeat_udata wd;
  heartbeat_udata ed;
} heartbeat_pow_context;

/**
 * Initialize a heartbeats instance.
 * Only fails if hb is NULL, window_size is 0, or window_buffer is NULL, in
 * which cases errno is set to EINVAL.
 *
 * @param hb
 * @param window_size
 * @param window_buffer
 * @param log_fd
 * @param hwc_callback
 * @return 0 on success, another value otherwise
 */
int heartbeat_pow_init(heartbeat_pow_context* hb,
                       uint64_t window_size,
                       heartbeat_pow_record* window_buffer,
                       int log_fd,
                       heartbeat_pow_window_complete* hwc_callback);

/**
 * Registers a heartbeat.
 * If hb is NULL or its window_buffer is NULL, errno is set to EINVAL.
 *
 * @param hb
 * @param user_tag
 * @param work
 * @param start_time (ns)
 * @param end_time (ns)
 * @param start_energy (uJ)
 * @param end_energy (uJ)
 */
void heartbeat_pow(heartbeat_pow_context* hb,
                   uint64_t user_tag,
                   uint64_t work,
                   uint64_t start_time,
                   uint64_t end_time,
                   uint64_t start_energy,
                   uint64_t end_energy);

/**
 * Write the header text to a log file.
 * Sets errno on failure.
 *
 * @param fd
 * @return 0 on success, error code otherwise
 */
int hb_pow_log_header(int fd);

/**
 * Logs the circular window buffer up to the current read index.
 * Sets errno on failure.
 *
 * @param hb
 * @param fd
 * @return 0 on success, error code otherwise
 */
int hb_pow_log_window_buffer(const heartbeat_pow_context* hb, int fd);

/**
 * Get the size of the window buffer.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the size of the window buffer
 */
uint64_t hb_pow_get_window_size(const heartbeat_pow_context* hb);

/**
 * Get the log file descriptor.
 * If hb is NULL, -1 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the log file descriptor
 */
int hb_pow_get_log_fd(const heartbeat_pow_context* hb);

/**
 * Get the user tag for the last heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the user tag
 */
uint64_t hb_pow_get_user_tag(const heartbeat_pow_context* hb);

/**
 * Get the total time (ns) for the life of this heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the total time (ns)
 */
uint64_t hb_pow_get_global_time(const heartbeat_pow_context* hb);

/**
 * Get the current window time (ns) for this heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the window time (ns)
 */
uint64_t hb_pow_get_window_time(const heartbeat_pow_context* hb);

/**
 * Get the total work for the life of this heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the total work
 */
uint64_t hb_pow_get_global_work(const heartbeat_pow_context* hb);

/**
 * Get the current window work for this heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the window work
 */
uint64_t hb_pow_get_window_work(const heartbeat_pow_context* hb);

/**
 * Get the performance over the life of this heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the performance over the life of this heartbeat
 */
double hb_pow_get_global_perf(const heartbeat_pow_context* hb);

/**
 * Get the performance over the last window for this heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the performance over the last window
 */
double hb_pow_get_window_perf(const heartbeat_pow_context* hb);

/**
 * Get the performance for the last heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the performance for the last heartbeat
 */
double hb_pow_get_instant_perf(const heartbeat_pow_context* hb);

/**
 * Get the total energy (uJ) for the life of this heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the total energy (uJ)
 */
uint64_t hb_pow_get_global_energy(const heartbeat_pow_context* hb);

/**
 * Get the current window energy (uJ) for this heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the window energy (uJ)
 */
uint64_t hb_pow_get_window_energy(const heartbeat_pow_context* hb);

/**
 * Get the power (Watts) over the life of this heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the power (Watts) over the life of this heartbeat
 */
double hb_pow_get_global_power(const heartbeat_pow_context* hb);

/**
 * Get the power (Watts) over the last window for this heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the power (Watts) over the last window
 */
double hb_pow_get_window_power(const heartbeat_pow_context* hb);

/**
 * Get the power (Watts) for the last heartbeat.
 * If hb is NULL, 0 is returned and errno is set to EINVAL.
 *
 * @param hb
 * @return the power (Watts) for the last heartbeat
 */
double hb_pow_get_instant_power(const heartbeat_pow_context* hb);

#ifdef __cplusplus
}
#endif

#endif
