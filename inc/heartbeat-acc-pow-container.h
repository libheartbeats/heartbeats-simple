/**
 * Container for a heartbeat and its window buffer, with utility functions for
 * memory management at initialization and completion..
 *
 * This version is for heartbeat-acc-pow.h.
 *
 * @author Connor Imes
 */
#ifndef _HEARTBEAT_ACC_POW_CONTAINER_H
#define _HEARTBEAT_ACC_POW_CONTAINER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "heartbeat-acc-pow.h"

typedef struct heartbeat_acc_pow_container {
  heartbeat_acc_pow_context hb;
  heartbeat_acc_pow_record* window_buffer;
} heartbeat_acc_pow_container;

/**
 * Allocate the window buffer.
 *
 * @param hc
 * @param window_size
 * @return 0 on success, another value otherwise
 */
int heartbeat_acc_pow_container_init(heartbeat_acc_pow_container* hc,
                                     uint64_t window_size);

/**
 * Convenience function to initialize the container and the heartbeat context.
 *
 * @param hc
 * @param window_size
 * @param log_fd
 * @param hwc_callback
 * @return 0 on success, another value otherwise
 */
int heartbeat_acc_pow_container_init_context(heartbeat_acc_pow_container* hc,
                                             uint64_t window_size,
                                             int log_fd,
                                             heartbeat_acc_pow_window_complete* hwc_callback);

/**
 * Free the window buffer.
 *
 * @param hc
 */
void heartbeat_acc_pow_container_finish(heartbeat_acc_pow_container* hc);

#ifdef __cplusplus
}
#endif

#endif
