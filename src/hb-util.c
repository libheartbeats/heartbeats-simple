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

#endif
