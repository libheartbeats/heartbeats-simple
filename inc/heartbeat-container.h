/**
 * Container for a heartbeat and its window buffer, with utility functions for
 * memory management at initialization and completion.
 *
 * This version is for heartbeat.h.
 *
 * @author Connor Imes
 */
#ifndef _HEARTBEAT_CONTAINER_H
#define _HEARTBEAT_CONTAINER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>
#include "heartbeat.h"

typedef struct heartbeat_container {
  heartbeat_context hb;
  heartbeat_record* window_buffer;
} heartbeat_container;

/**
 * Allocate the window buffer and store the file descriptor.
 * Sets errno on failure.
 *
 * @param hc
 * @param window_size
 * @return 0 on success, another value otherwise
 */
int heartbeat_container_init(heartbeat_container* hc,
                             uint64_t window_size);

/**
 * Convenience function to initialize the container and the heartbeat context.
 * Sets errno on failure.
 *
 * @param hc
 * @param window_size
 * @param log_fd
 * @param hwc_callback
 * @return 0 on success, another value otherwise
 */
int heartbeat_container_init_context(heartbeat_container* hc,
                                     uint64_t window_size,
                                     int log_fd,
                                     heartbeat_window_complete* hwc_callback);

/**
 * Free the window buffer.
 *
 * @param hc
 */
void heartbeat_container_finish(heartbeat_container* hc);

#ifdef __cplusplus
}
#endif

#endif
