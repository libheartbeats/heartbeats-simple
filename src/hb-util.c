/**
 * Functions defined in heartbeat header files that are reusable across
 * implementations due to common structure in heartbeat_t structs.
 *
 * To disable function definitions for a particular heartbeat interface and
 * implement them elsewhere, define the following macros as needed:
 *   HEARTBEAT_UTIL_OVERRIDE
 *   HEARTBEAT_POWER_UTIL_OVERRIDE
 *
 * @author Connor Imes
 * @author Hank Hoffmann
 */

#include <string.h>
#include <inttypes.h>

/* Determine which heartbeat implementation to use */
#if defined(HEARTBEAT_MODE_POW)
#include "heartbeat-pow.h"
#include "heartbeat-pow-types.h"
#else
#include "heartbeat.h"
#include "heartbeat-types.h"
#endif

/*
 * Functions from heartbeat.h
 */
#if !defined(HEARTBEAT_UTIL_OVERRIDE)

uint64_t hb_get_window_size(const heartbeat_t* hb) {
  return hb->window_size;
}

void hb_get_current(const heartbeat_t* hb,
                    heartbeat_record_t* record) {
  hb_get_history(hb, record, 1);
}

uint64_t hb_get_user_tag(const heartbeat_t* hb) {
  return hb->window_buffer[hb->read_index].user_tag;
}

uint64_t hb_get_global_time(const heartbeat_t* hb) {
  return hb->td.total_time;
}

uint64_t hb_get_window_time(const heartbeat_t* hb) {
  return hb->td.window_time;
}

uint64_t hb_get_global_work(const heartbeat_t* hb) {
  return hb->wd.total_work;
}

uint64_t hb_get_window_work(const heartbeat_t* hb) {
  return hb->wd.window_work;
}

double hb_get_global_rate(const heartbeat_t* hb) {
  return hb->window_buffer[hb->read_index].global_perf;
}

double hb_get_window_rate(const heartbeat_t* hb) {
  return hb->window_buffer[hb->read_index].window_perf;
}

double hb_get_instant_rate(const heartbeat_t* hb) {
  return hb->window_buffer[hb->read_index].instant_perf;
}

uint64_t hb_get_history(const heartbeat_t* hb,
                        heartbeat_record_t* record,
                        uint64_t n) {
  if (n == 0) {
    return 0;
  }

  if (n > hb->counter) {
    // more records were requested than have been created
    memcpy(record,
           &hb->window_buffer[0],
           hb->buffer_index * sizeof(heartbeat_record_t));
    return hb->buffer_index;
  }

  if (hb->buffer_index >= n) {
    // the number of records requested do not overflow the circular buffer
    memcpy(record,
           &hb->window_buffer[hb->buffer_index - n],
           n * sizeof(heartbeat_record_t));
    return n;
  }

  // the number of records requested could overflow the circular buffer
  if (n >= hb->window_size) {
    // more records were requested than we can support, return what we have
    memcpy(record,
         &hb->window_buffer[hb->buffer_index],
         (hb->window_size - hb->buffer_index) * sizeof(heartbeat_record_t));
    memcpy(record + hb->window_size - hb->buffer_index,
           &hb->window_buffer[0],
           hb->buffer_index * sizeof(heartbeat_record_t));
    return hb->window_size;
  }

  // buffer_index < n < window_size
  // still overflows circular buffer, but we don't want all records
  memcpy(record,
         &hb->window_buffer[hb->window_size + hb->buffer_index - n],
         (n - hb->buffer_index) * sizeof(heartbeat_record_t));
  memcpy(record + n - hb->buffer_index,
         &hb->window_buffer[0],
         hb->buffer_index * sizeof(heartbeat_record_t));
  return n;
}

uint64_t hbr_get_beat_number(const heartbeat_record_t* hbr) {
  return hbr->id;
}

uint64_t hbr_get_user_tag(const heartbeat_record_t* hbr) {
  return hbr->user_tag;
}

uint64_t hbr_get_work(const heartbeat_record_t* hbr) {
  return hbr->work;
}

uint64_t hbr_get_start_time(const heartbeat_record_t* hbr) {
  return hbr->start_time;
}

uint64_t hbr_get_end_time(const heartbeat_record_t* hbr) {
  return hbr->end_time;
}

double hbr_get_global_rate(const heartbeat_record_t* hbr) {
  return hbr->global_perf;
}

double hbr_get_window_rate(const heartbeat_record_t* hbr) {
  return hbr->window_perf;
}

double hbr_get_instant_rate(const heartbeat_record_t* hbr) {
  return hbr->instant_perf;
}

#endif

/*
 * Functions from heartbeat-pow.h
 */
#if defined(HEARTBEAT_MODE_POW) && !defined(HEARTBEAT_POWER_UTIL_OVERRIDE)

uint64_t hb_get_global_energy(const heartbeat_t* hb) {
  return hb->ed.total_energy;
}

uint64_t hb_get_window_energy(const heartbeat_t* hb) {
  return hb->ed.window_energy;
}

double hb_get_global_power(const heartbeat_t* hb) {
  return hb->window_buffer[hb->read_index].global_pwr;
}

double hb_get_window_power(const heartbeat_t* hb) {
  return hb->window_buffer[hb->read_index].window_pwr;
}

double hb_get_instant_power(const heartbeat_t* hb) {
  return hb->window_buffer[hb->read_index].instant_pwr;
}

uint64_t hbr_get_start_energy(const heartbeat_record_t* hbr) {
  return hbr->start_energy;
}

uint64_t hbr_get_end_energy(const heartbeat_record_t* hbr) {
  return hbr->end_energy;
}

double hbr_get_global_power(const heartbeat_record_t* hbr) {
  return hbr->global_pwr;
}

double hbr_get_window_power(const heartbeat_record_t* hbr) {
  return hbr->window_pwr;
}

double hbr_get_instant_power(const heartbeat_record_t* hbr) {
  return hbr->instant_pwr;
}

#endif
