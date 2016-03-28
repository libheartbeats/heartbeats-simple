/**
 * Simple heartbeats implementation(s).
 *
 * @author Connor Imes
 * @date 2015-07-15
 */
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Determine which heartbeat implementation to use */
#if defined(HEARTBEAT_MODE_ACC)
#include "heartbeat-acc.h"
#elif defined(HEARTBEAT_MODE_POW)
#include "heartbeat-pow.h"
#elif defined(HEARTBEAT_MODE_ACC_POW)
#include "heartbeat-acc-pow.h"
#else
#include "heartbeat.h"
#endif

#define __STDC_FORMAT_MACROS

static inline void init_udata(heartbeat_udata* data) {
  data->global = 0;
  data->window = 0;
}

#if defined(HEARTBEAT_MODE_ACC)
int heartbeat_acc_init(heartbeat_acc_context* hb,
                       uint64_t window_size,
                       heartbeat_acc_record* window_buffer,
                       int log_fd,
                       heartbeat_acc_window_complete* hwc_callback) {
  size_t record_size = sizeof(heartbeat_acc_record);
#elif defined(HEARTBEAT_MODE_POW)
int heartbeat_pow_init(heartbeat_pow_context* hb,
                       uint64_t window_size,
                       heartbeat_pow_record* window_buffer,
                       int log_fd,
                       heartbeat_pow_window_complete* hwc_callback) {
  size_t record_size = sizeof(heartbeat_pow_record);
#elif defined(HEARTBEAT_MODE_ACC_POW)
int heartbeat_acc_pow_init(heartbeat_acc_pow_context* hb,
                           uint64_t window_size,
                           heartbeat_acc_pow_record* window_buffer,
                           int log_fd,
                           heartbeat_acc_pow_window_complete* hwc_callback) {
  size_t record_size = sizeof(heartbeat_acc_pow_record);
#else
int heartbeat_init(heartbeat_context* hb,
                   uint64_t window_size,
                   heartbeat_record* window_buffer,
                   int log_fd,
                   heartbeat_window_complete* hwc_callback) {
  size_t record_size = sizeof(heartbeat_record);
#endif
  if (hb == NULL || window_buffer == NULL || window_size == 0) {
    errno = EINVAL;
    return -1;
  }

  hb->ws.buffer_index = 0;
  hb->ws.read_index = 0;
  hb->ws.window_size = window_size;
  hb->ws.log_fd = log_fd;
  hb->window_buffer = window_buffer;
  // cheap way to set initial values to 0 (necessary for managing window data)
  memset(hb->window_buffer, 0, window_size * record_size);
  hb->counter = 0;
  hb->lock = 0;
  hb->hwc_callback = hwc_callback;
  init_udata(&hb->td);
  init_udata(&hb->wd);
#if defined(HEARTBEAT_USE_ACC)
  init_udata(&hb->ad);
#endif
#if defined(HEARTBEAT_USE_POW)
  init_udata(&hb->ed);
#endif

  return 0;
}

#if defined(HEARTBEAT_MODE_ACC)
int hb_acc_log_header(int fd) {
#elif defined(HEARTBEAT_MODE_POW)
int hb_pow_log_header(int fd) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
int hb_acc_pow_log_header(int fd) {
#else
int hb_log_header(int fd) {
#endif
  int err_save;
  FILE* log = fdopen(dup(fd), "w");

  if (log == NULL) {
    return errno;
  }

  errno = 0;
  fprintf(log,
          "%-6s %-6s"
          " %-11s %-11s %-11s"
          " %-15s %-15s %-20s %-20s"
          " %-15s %-15s %-15s"
#if defined(HEARTBEAT_USE_ACC)
          " %-11s %-11s %-11s"
          " %-16s %-16s %-16s"
#endif
#if defined(HEARTBEAT_USE_POW)
          " %-15s %-15s %-15s %-15s"
          " %-15s %-15s %-15s"
#endif
          "\n",
          "HB", "Tag",
          "Global_Work", "Window_Work", "Work",
          "Global_Time", "Window_Time", "Start_Time", "End_Time",
          "Global_Perf", "Window_Perf", "Instant_Perf"
#if defined(HEARTBEAT_USE_ACC)
          ,
          "Global_Acc", "Window_Acc", "Acc",
          "Global_Acc_Rate", "Window_Acc_Rate", "Instant_Acc_Rate"
#endif
#if defined(HEARTBEAT_USE_POW)
          ,
          "Global_Energy", "Window_Energy", "Start_Energy", "End_Energy",
          "Global_Pwr", "Window_Pwr", "Instant_Pwr"
#endif
  );
  err_save = errno;
  fclose(log);
  // preserve first error
  errno = err_save ? err_save : errno;
  return errno;
}

#if defined(HEARTBEAT_MODE_ACC)
int hb_acc_log_window_buffer(const heartbeat_acc_context* hb, int fd) {
#elif defined(HEARTBEAT_MODE_POW)
int hb_pow_log_window_buffer(const heartbeat_pow_context* hb, int fd) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
int hb_acc_pow_log_window_buffer(const heartbeat_acc_pow_context* hb, int fd) {
#else
int hb_log_window_buffer(const heartbeat_context* hb, int fd) {
#endif
  if (hb == NULL) {
    errno = EINVAL;
    return errno;
  }

  int err_save;
  uint64_t i;
  FILE* log = fdopen(dup(fd), "w");

  if (log == NULL) {
    return errno;
  }

  errno = 0;
  for (i = 0; i < hb->ws.buffer_index && !errno; i++) {
    fprintf(log,
            "%-6"PRIu64" %-6"PRIu64
            " %-11"PRIu64" %-11"PRIu64" %-11"PRIu64
            " %-15"PRIu64" %-15"PRIu64" %-20"PRIu64" %-20"PRIu64
            " %-15.6f %-15.6f %-15.6f"
#if defined(HEARTBEAT_USE_ACC)
            " %-11"PRIu64" %-11"PRIu64" %-11"PRIu64
            " %-16.6f %-16.6f %-16.6f"
#endif
#if defined(HEARTBEAT_USE_POW)
            " %-15"PRIu64" %-15"PRIu64" %-15"PRIu64" %-15"PRIu64
            " %-15.6f %-15.6f %-15.6f"
#endif
            "\n",
            hb->window_buffer[i].id,
            hb->window_buffer[i].user_tag,

            hb->window_buffer[i].wd.global,
            hb->window_buffer[i].wd.window,
            hb->window_buffer[i].work,

            hb->window_buffer[i].td.global,
            hb->window_buffer[i].td.window,
            hb->window_buffer[i].start_time,
            hb->window_buffer[i].end_time,

            hb->window_buffer[i].perf.global,
            hb->window_buffer[i].perf.window,
            hb->window_buffer[i].perf.instant
#if defined(HEARTBEAT_USE_ACC)
            ,
            hb->window_buffer[i].ad.global,
            hb->window_buffer[i].ad.window,
            hb->window_buffer[i].accuracy,

            hb->window_buffer[i].acc.global,
            hb->window_buffer[i].acc.window,
            hb->window_buffer[i].acc.instant
#endif
#if defined(HEARTBEAT_USE_POW)
            ,
            hb->window_buffer[i].ed.global,
            hb->window_buffer[i].ed.window,
            hb->window_buffer[i].start_energy,
            hb->window_buffer[i].end_energy,

            hb->window_buffer[i].pwr.global,
            hb->window_buffer[i].pwr.window,
            hb->window_buffer[i].pwr.instant
#endif
    );
  }
  err_save = errno;
  fclose(log);
  // preserve first error
  errno = err_save ? err_save : errno;
  return errno;
}

#define ONE_MILLION 1000000.0
#define ONE_BILLION 1000000000.0

#if defined(HEARTBEAT_MODE_ACC)
void heartbeat_acc(heartbeat_acc_context* hb,
                   uint64_t user_tag,
                   uint64_t work,
                   uint64_t start_time,
                   uint64_t end_time,
                   uint64_t accuracy) {
  heartbeat_acc_record* old_record;
#elif defined(HEARTBEAT_MODE_POW)
void heartbeat_pow(heartbeat_pow_context* hb,
                   uint64_t user_tag,
                   uint64_t work,
                   uint64_t start_time,
                   uint64_t end_time,
                   uint64_t start_energy,
                   uint64_t end_energy) {
  heartbeat_pow_record* old_record;
#elif defined(HEARTBEAT_MODE_ACC_POW)
void heartbeat_acc_pow(heartbeat_acc_pow_context* hb,
                       uint64_t user_tag,
                       uint64_t work,
                       uint64_t start_time,
                       uint64_t end_time,
                       uint64_t accuracy,
                       uint64_t start_energy,
                       uint64_t end_energy) {
  heartbeat_acc_pow_record* old_record;
#else
void heartbeat(heartbeat_context* hb,
               uint64_t user_tag,
               uint64_t work,
               uint64_t start_time,
               uint64_t end_time) {
  heartbeat_record* old_record;
#endif
  if (hb == NULL || hb->window_buffer == NULL) {
    errno = EINVAL;
    return;
  }

  while (__sync_lock_test_and_set(&hb->lock, 1)) {
    while (hb->lock);
  }

  // if we haven't yet reached window_size heartbeats, the log values are 0
  old_record = &hb->window_buffer[hb->ws.buffer_index];

  hb->window_buffer[hb->ws.buffer_index].id = hb->counter;
  hb->window_buffer[hb->ws.buffer_index].user_tag = user_tag;

  // time and work
  int64_t delta_time = end_time - start_time;
  hb->td.global += delta_time;
  hb->td.window = hb->td.global - old_record->td.global;
  hb->wd.global += work;
  hb->wd.window = hb->wd.global - old_record->wd.global;
  hb->window_buffer[hb->ws.buffer_index].work = work;
  memcpy(&hb->window_buffer[hb->ws.buffer_index].wd, &hb->wd, sizeof(heartbeat_udata));
  hb->window_buffer[hb->ws.buffer_index].start_time = start_time;
  hb->window_buffer[hb->ws.buffer_index].end_time = end_time;
  memcpy(&hb->window_buffer[hb->ws.buffer_index].td, &hb->td, sizeof(heartbeat_udata));
  double total_seconds = ((double) hb->td.global) / ONE_BILLION;
  double window_seconds = ((double) hb->td.window) / ONE_BILLION;
  double instant_seconds = ((double) delta_time) / ONE_BILLION;
  hb->window_buffer[hb->ws.buffer_index].perf.global = ((double) hb->wd.global) / total_seconds;
  hb->window_buffer[hb->ws.buffer_index].perf.window = ((double) hb->wd.window) / window_seconds;
  hb->window_buffer[hb->ws.buffer_index].perf.instant = ((double) work) / instant_seconds;

#if defined(HEARTBEAT_USE_ACC)
  // accuracy
  hb->ad.global += accuracy;
  hb->ad.window = hb->ad.global - old_record->ad.global;
  hb->window_buffer[hb->ws.buffer_index].accuracy = accuracy;
  memcpy(&hb->window_buffer[hb->ws.buffer_index].ad, &hb->ad, sizeof(heartbeat_udata));
  hb->window_buffer[hb->ws.buffer_index].acc.global = ((double) hb->ad.global) / total_seconds;
  hb->window_buffer[hb->ws.buffer_index].acc.window = ((double) hb->ad.window) / window_seconds;
  hb->window_buffer[hb->ws.buffer_index].acc.instant = ((double) accuracy) / instant_seconds;
#endif

#if defined(HEARTBEAT_USE_POW)
  // energy
  int64_t delta_energy = end_energy - start_energy;
  hb->ed.global += delta_energy;
  hb->ed.window = hb->ed.global - old_record->ed.global;
  hb->window_buffer[hb->ws.buffer_index].start_energy = start_energy;
  hb->window_buffer[hb->ws.buffer_index].end_energy = end_energy;
  memcpy(&hb->window_buffer[hb->ws.buffer_index].ed, &hb->ed, sizeof(heartbeat_udata));
  hb->window_buffer[hb->ws.buffer_index].pwr.global = ((double) hb->ed.global) / total_seconds / ONE_MILLION;
  hb->window_buffer[hb->ws.buffer_index].pwr.window = ((double) hb->ed.window) / window_seconds / ONE_MILLION;
  hb->window_buffer[hb->ws.buffer_index].pwr.instant = ((double) delta_energy) / instant_seconds / ONE_MILLION;
#endif

  // update context state
  hb->counter++;
  hb->ws.read_index = hb->ws.buffer_index;
  hb->ws.buffer_index++;
  // check circular buffer, issue callback if full
  if (hb->ws.buffer_index % hb->ws.window_size == 0) {
    if (hb->ws.log_fd > 0) {
#if defined(HEARTBEAT_MODE_ACC)
      if (hb_acc_log_window_buffer(hb, hb->ws.log_fd)) {
#elif defined(HEARTBEAT_MODE_POW)
      if (hb_pow_log_window_buffer(hb, hb->ws.log_fd)) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
      if (hb_acc_pow_log_window_buffer(hb, hb->ws.log_fd)) {
#else
      if (hb_log_window_buffer(hb, hb->ws.log_fd)) {
#endif
        perror("Failed to log heartbeat record data");
      }
    }
    if (hb->hwc_callback != NULL) {
      (*hb->hwc_callback)(hb);
    }
    hb->ws.buffer_index = 0;
  }

  __sync_lock_release(&hb->lock);
}
