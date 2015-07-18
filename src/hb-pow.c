/**
 * Implementation of heartbeat-pow.h
 *
 * @author Connor Imes
 * @date 2015-07-15
 */
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "heartbeat-pow.h"

#define __STDC_FORMAT_MACROS

static inline void init_udata(heartbeat_udata* data) {
  data->global = 0;
  data->window = 0;
}

int heartbeat_init(heartbeat_context* hb,
                   uint64_t window_size,
                   heartbeat_record* window_buffer,
                   heartbeat_window_complete* hwc_callback) {
  if (hb == NULL || window_buffer == NULL || window_size == 0) {
    return 1;
  }

  hb->counter = 0;
  hb->buffer_index = 0;
  hb->read_index = 0;
  hb->window_size = window_size;
  hb->lock = 0;
  init_udata(&hb->td);
  init_udata(&hb->wd);
  init_udata(&hb->ed);
  hb->window_buffer = window_buffer;
  // cheap way to set initial values to 0 (necessary for managing window data)
  memset(hb->window_buffer, 0, window_size * sizeof(heartbeat_record));
  hb->hwc_callback = hwc_callback;

  return 0;
}

int heartbeat_log_window_buffer(const heartbeat_context* hb, int fd, int print_header) {
  int ret;
  uint64_t i;
  FILE* log = fdopen(fd, "w");
  if (log == NULL) {
    perror("Failed to open log file for writing");
    ret = 1;
  } else {
    if (print_header) {
      fprintf(log,
              "HB    Tag    "
              "Work    Start_Time    End_time    Global_Perf    Window_Perf    Instant_Perf    "
              "Start_Energy    End_Energy    Global_Pwr    Window_Pwr    Instant_Pwr\n");
    }
    for (i = 0; i < hb->buffer_index; i++) {
      fprintf(log,
              "%" PRIu64"    %" PRIu64"    "
              "%" PRIu64"    %" PRIu64"    %" PRIu64"    %f    %f    %f    "
              "%" PRIu64"    %" PRIu64"    %f    %f    %f\n",
              hb->window_buffer[i].id,
              hb->window_buffer[i].user_tag,

              hb->window_buffer[i].work,
              hb->window_buffer[i].start_time,
              hb->window_buffer[i].end_time,
              hb->window_buffer[i].global_perf,
              hb->window_buffer[i].window_perf,
              hb->window_buffer[i].instant_perf,

              hb->window_buffer[i].start_energy,
              hb->window_buffer[i].end_energy,
              hb->window_buffer[i].global_pwr,
              hb->window_buffer[i].window_pwr,
              hb->window_buffer[i].instant_pwr);
    }
    ret = fflush(log);
  }
  return ret;
}

void heartbeat_pow(heartbeat_context* hb,
                   uint64_t user_tag,
                   uint64_t work,
                   uint64_t start_time,
                   uint64_t end_time,
                   uint64_t start_energy,
                   uint64_t end_energy) {
  // these could be <= 0, but it's not actually harmful
  int64_t delta_time = end_time - start_time;
  int64_t delta_energy = end_energy - start_energy;
  const double one_billion = 1000000000.0;
  const double one_million = 1000000.0;

  while (__sync_lock_test_and_set(&hb->lock, 1)) {
    while (hb->lock);
  }

  // update total data
  hb->td.global += delta_time;
  hb->wd.global += work;
  hb->ed.global += delta_energy;

  // update the sliding window values
  // if we haven't yet reached window_size heartbeats, the log values are 0
  heartbeat_record* old_record = &hb->window_buffer[hb->buffer_index];
  hb->td.window += delta_time - (old_record->end_time - old_record->start_time);
  hb->wd.window += work - old_record->work;
  hb->ed.window += delta_energy - (old_record->end_energy - old_record->start_energy);

  double total_seconds = ((double) hb->td.global) / one_billion;
  double window_seconds = ((double) hb->td.window) / one_billion;
  double instant_seconds = ((double) delta_time) / one_billion;

  // store in log
  hb->window_buffer[hb->buffer_index].id = hb->counter;
  hb->window_buffer[hb->buffer_index].user_tag = user_tag;
  hb->window_buffer[hb->buffer_index].work = work;
  hb->window_buffer[hb->buffer_index].start_time = start_time;
  hb->window_buffer[hb->buffer_index].end_time = end_time;
  hb->window_buffer[hb->buffer_index].start_energy = start_energy;
  hb->window_buffer[hb->buffer_index].end_energy = end_energy;
  hb->window_buffer[hb->buffer_index].global_perf = ((double) hb->wd.global) / total_seconds;
  hb->window_buffer[hb->buffer_index].window_perf = ((double) hb->wd.window) / window_seconds;
  hb->window_buffer[hb->buffer_index].instant_perf = ((double) work) / instant_seconds;
  hb->window_buffer[hb->buffer_index].global_pwr = ((double) hb->ed.global) / total_seconds / one_million;
  hb->window_buffer[hb->buffer_index].window_pwr = ((double) hb->ed.window) / window_seconds / one_million;
  hb->window_buffer[hb->buffer_index].instant_pwr = ((double) delta_energy) / instant_seconds / one_million;

  // update context state
  hb->counter++;
  hb->read_index = hb->buffer_index;
  hb->buffer_index++;
  // check circular buffer, issue callback if full
  if (hb->buffer_index % hb->window_size == 0) {
    if (hb->hwc_callback != NULL) {
      (*hb->hwc_callback)(hb, hb->window_buffer, hb->window_size);
    }
    hb->buffer_index = 0;
  }

  __sync_lock_release(&hb->lock);
}

void heartbeat(heartbeat_context* hb,
               uint64_t user_tag,
               uint64_t work,
               uint64_t start_time,
               uint64_t end_time) {
  heartbeat_pow(hb, user_tag, work, start_time, end_time, 0, 0);
}
