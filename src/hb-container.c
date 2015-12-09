/**
 * Convenience functions for managing a heartbeat and its dynamically
 * allocated window buffer.
 *
 * @author Connor Imes
 */
#include <errno.h>
#include <inttypes.h>
#include <stdlib.h>

/* Determine which heartbeat implementation to use */
#if defined(HEARTBEAT_MODE_ACC)
#include "heartbeat-acc-container.h"
#elif defined(HEARTBEAT_MODE_POW)
#include "heartbeat-pow-container.h"
#elif defined(HEARTBEAT_MODE_ACC_POW)
#include "heartbeat-acc-pow-container.h"
#else
#include "heartbeat-container.h"
#endif

#if defined(HEARTBEAT_MODE_ACC)
int heartbeat_acc_container_init(heartbeat_acc_container* hc,
                                 uint64_t window_size) {
  size_t record_size = sizeof(heartbeat_acc_record);
#elif defined(HEARTBEAT_MODE_POW)
int heartbeat_pow_container_init(heartbeat_pow_container* hc,
                                 uint64_t window_size) {
  size_t record_size = sizeof(heartbeat_pow_record);
#elif defined(HEARTBEAT_MODE_ACC_POW)
int heartbeat_acc_pow_container_init(heartbeat_acc_pow_container* hc,
                                     uint64_t window_size) {
  size_t record_size = sizeof(heartbeat_acc_pow_record);
#else
int heartbeat_container_init(heartbeat_container* hc,
                             uint64_t window_size) {
  size_t record_size = sizeof(heartbeat_record);
#endif
  if (hc == NULL) {
    errno = EINVAL;
    return -1;
  }
  hc->window_buffer = malloc(window_size * record_size);
  if (hc->window_buffer == NULL) {
    return -1;
  }
  return 0;
}

#if defined(HEARTBEAT_MODE_ACC)
int heartbeat_acc_container_init_context(heartbeat_acc_container* hc,
                                         uint64_t window_size,
                                         int log_fd,
                                         heartbeat_acc_window_complete* hwc_callback) {
  if (heartbeat_acc_container_init(hc, window_size)) {
    return -1;
  }
  if (heartbeat_acc_init(&hc->hb,
                         window_size,
                         hc->window_buffer,
                         log_fd,
                         hwc_callback)) {
    heartbeat_acc_container_finish(hc);
    return -1;
  }
  return 0;
}
#elif defined(HEARTBEAT_MODE_POW)
int heartbeat_pow_container_init_context(heartbeat_pow_container* hc,
                                         uint64_t window_size,
                                         int log_fd,
                                         heartbeat_pow_window_complete* hwc_callback) {
  if (heartbeat_pow_container_init(hc, window_size)) {
    return -1;
  }
  if (heartbeat_pow_init(&hc->hb,
                         window_size,
                         hc->window_buffer,
                         log_fd,
                         hwc_callback)) {
    heartbeat_pow_container_finish(hc);
    return -1;
  }
  return 0;
}
#elif defined(HEARTBEAT_MODE_ACC_POW)
int heartbeat_acc_pow_container_init_context(heartbeat_acc_pow_container* hc,
                                             uint64_t window_size,
                                             int log_fd,
                                             heartbeat_acc_pow_window_complete* hwc_callback) {
  if (heartbeat_acc_pow_container_init(hc, window_size)) {
    return -1;
  }
  if (heartbeat_acc_pow_init(&hc->hb,
                             window_size,
                             hc->window_buffer,
                             log_fd,
                             hwc_callback)) {
    heartbeat_acc_pow_container_finish(hc);
    return -1;
  }
  return 0;
}
#else
int heartbeat_container_init_context(heartbeat_container* hc,
                                     uint64_t window_size,
                                     int log_fd,
                                     heartbeat_window_complete* hwc_callback) {
  if (heartbeat_container_init(hc, window_size)) {
    return -1;
  }
  if (heartbeat_init(&hc->hb,
                     window_size,
                     hc->window_buffer,
                     log_fd,
                     hwc_callback)) {
    heartbeat_container_finish(hc);
    return -1;
  }
  return 0;
}
#endif

#if defined(HEARTBEAT_MODE_ACC)
void heartbeat_acc_container_finish(heartbeat_acc_container* hc) {
#elif defined(HEARTBEAT_MODE_POW)
void heartbeat_pow_container_finish(heartbeat_pow_container* hc) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
void heartbeat_acc_pow_container_finish(heartbeat_acc_pow_container* hc) {
#else
void heartbeat_container_finish(heartbeat_container* hc) {
#endif
  if (hc != NULL) {
    free(hc->window_buffer);
    hc->window_buffer = NULL;
  }
}
