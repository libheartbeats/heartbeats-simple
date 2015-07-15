/**
 * Implementation of heartbeat-pow.h
 *
 * @author Connor Imes
 */
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "heartbeat-pow.h"

#define __STDC_FORMAT_MACROS

static inline void init_time_data(heartbeat_time_data* td) {
  td->total_time = 0;
  td->window_time = 0;
}

static inline void init_work_data(heartbeat_work_data* td) {
  td->total_work = 0;
  td->window_work = 0;
}

static inline void init_energy_data(heartbeat_energy_data* ed) {
  ed->total_energy = 0;
  ed->window_energy = 0;
}

int heartbeat_init(heartbeat_t* hb,
                   uint64_t window_size,
                   heartbeat_record_t* window_buffer,
                   heartbeat_window_complete* hwc_callback) {
  if (hb == NULL || window_buffer == NULL || window_size <= 0) {
    return 1;
  }

  hb->counter = 0;
  hb->buffer_index = 0;
  hb->read_index = 0;
  hb->window_size = window_size;
  init_time_data(&hb->td);
  init_work_data(&hb->wd);
  init_energy_data(&hb->ed);
  hb->window_buffer = window_buffer;
  // cheap way to set initial values to 0 (necessary for managing window data)
  memset(hb->window_buffer, 0, window_size * sizeof(heartbeat_record_t));
  hb->hwc_callback = hwc_callback;

  return 0;
}

int heartbeat_log_window_buffer(const heartbeat_t* hb, FILE* log, int print_header) {
  int ret = 1;
  uint64_t i;
  if (log != NULL) {
    if (print_header) {
      fprintf(log,
              "HB    Tag    "
              "Work    Start_Time    End_time    Global_Perf    Window_Perf    Instant_Perf    "
              "Start_Energy    End_Energy    Global_Pwr    Window_Pwr    Instant_Pwr\n");
    }
    for (i = 0; i < hb->buffer_index; i++) {
      fprintf(log,
              "%" PRIu64"    %" PRIu64"    "
              "%" PRIu64"    %" PRIi64"    %" PRIi64"    %f    %f    %f    "
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

void heartbeat_pow(heartbeat_t* hb,
                   uint64_t user_tag,
                   uint64_t work,
                   int64_t start_time,
                   int64_t end_time,
                   uint64_t start_energy,
                   uint64_t end_energy) {
  int64_t delta_time = end_time - start_time;
  uint64_t delta_energy = end_energy - start_energy;

  // update total data
  hb->td.total_time += delta_time;
  hb->wd.total_work += work;
  hb->ed.total_energy += delta_energy;

  // now update the running window values
  // if we haven't yet reached window_size heartbeats, the log values are 0
  heartbeat_record_t* old_record = &hb->window_buffer[hb->buffer_index];
  hb->td.window_time += delta_time - (old_record->end_time - old_record->start_time);
  hb->wd.window_work += work - old_record->work;
  hb->ed.window_energy += delta_energy - (old_record->end_energy - old_record->start_energy);

  hb->counter++;
  hb->read_index = hb->buffer_index;
  uint64_t index = hb->buffer_index;
  hb->buffer_index++;

  // now store in log
  hb->window_buffer[index].id = hb->counter - 1;
  hb->window_buffer[index].user_tag = user_tag;
  hb->window_buffer[index].work = work;
  hb->window_buffer[index].start_time = start_time;
  hb->window_buffer[index].end_time = end_time;
  hb->window_buffer[index].start_energy = start_energy;
  hb->window_buffer[index].end_energy = end_energy;
  if (delta_time == 0) {
    hb->window_buffer[index].global_perf = 0;
    hb->window_buffer[index].window_perf = 0;
    hb->window_buffer[index].instant_perf = 0;
    hb->window_buffer[index].global_pwr = 0;
    hb->window_buffer[index].window_pwr = 0;
    hb->window_buffer[index].instant_pwr = 0;
  } else {
    const double one_billion = 1000000000.0;
    const double one_million = 1000000.0;
    double total_seconds = ((double) hb->td.total_time) / one_billion;
    double window_seconds = ((double) hb->td.window_time) / one_billion;
    double instant_seconds = ((double) delta_time) / one_billion;
    hb->window_buffer[index].global_perf = ((double) hb->wd.total_work) / total_seconds;
    hb->window_buffer[index].window_perf = ((double) hb->wd.window_work) / window_seconds;
    hb->window_buffer[index].instant_perf = ((double) work) / instant_seconds;
    hb->window_buffer[index].global_pwr = ((double) hb->ed.total_energy) / total_seconds / one_million;
    hb->window_buffer[index].window_pwr = ((double) hb->ed.window_energy) / window_seconds / one_million;
    hb->window_buffer[index].instant_pwr = ((double) delta_energy) / instant_seconds / one_million;
  }

  // check circular buffer, issue callback if full
  if (hb->buffer_index % hb->window_size == 0) {
    if (hb->hwc_callback != NULL) {
      (*hb->hwc_callback)(hb, hb->window_buffer, hb->window_size);
    }
    hb->buffer_index = 0;
  }
}

void heartbeat(heartbeat_t* hb,
               uint64_t user_tag,
               uint64_t work,
               int64_t start_time,
               int64_t end_time) {
  heartbeat_pow(hb, user_tag, work, start_time, end_time, 0, 0);
}
