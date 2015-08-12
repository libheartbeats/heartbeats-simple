/**
 * Functions defined in heartbeat header files that are reusable across
 * implementations due to common structure in heartbeat structs.
 *
 * @author Connor Imes
 */

#include <inttypes.h>
#include <stdlib.h>

/* Determine which heartbeat implementation to use */
#if defined(HEARTBEAT_MODE_POW)
#include "heartbeat-pow.h"
#elif defined(HEARTBEAT_MODE_ACC)
#include "heartbeat-acc.h"
#elif defined(HEARTBEAT_MODE_ACC_POW)
#include "heartbeat-acc-pow.h"
#else
#include "heartbeat.h"
#endif

#if defined(HEARTBEAT_MODE_ACC)
uint64_t hb_acc_get_window_size(const heartbeat_acc_context* hb) {
#elif defined(HEARTBEAT_MODE_POW)
uint64_t hb_pow_get_window_size(const heartbeat_pow_context* hb) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
uint64_t hb_acc_pow_get_window_size(const heartbeat_acc_pow_context* hb) {
#else
uint64_t hb_get_window_size(const heartbeat_context* hb) {
#endif
  return hb == NULL ? 0 : hb->ws.window_size;
}

#if defined(HEARTBEAT_MODE_ACC)
uint64_t hb_acc_get_user_tag(const heartbeat_acc_context* hb) {
#elif defined(HEARTBEAT_MODE_POW)
uint64_t hb_pow_get_user_tag(const heartbeat_pow_context* hb) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
uint64_t hb_acc_pow_get_user_tag(const heartbeat_acc_pow_context* hb) {
#else
uint64_t hb_get_user_tag(const heartbeat_context* hb) {
#endif
  return hb == NULL ? 0 : hb->window_buffer[hb->ws.read_index].user_tag;
}

#if defined(HEARTBEAT_MODE_ACC)
uint64_t hb_acc_get_global_time(const heartbeat_acc_context* hb) {
#elif defined(HEARTBEAT_MODE_POW)
uint64_t hb_pow_get_global_time(const heartbeat_pow_context* hb) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
uint64_t hb_acc_pow_get_global_time(const heartbeat_acc_pow_context* hb) {
#else
uint64_t hb_get_global_time(const heartbeat_context* hb) {
#endif
  return hb == NULL ? 0 : hb->td.global;
}

#if defined(HEARTBEAT_MODE_ACC)
uint64_t hb_acc_get_window_time(const heartbeat_acc_context* hb) {
#elif defined(HEARTBEAT_MODE_POW)
uint64_t hb_pow_get_window_time(const heartbeat_pow_context* hb) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
uint64_t hb_acc_pow_get_window_time(const heartbeat_acc_pow_context* hb) {
#else
uint64_t hb_get_window_time(const heartbeat_context* hb) {
#endif
  return hb == NULL ? 0 : hb->td.window;
}

#if defined(HEARTBEAT_MODE_ACC)
uint64_t hb_acc_get_global_work(const heartbeat_acc_context* hb) {
#elif defined(HEARTBEAT_MODE_POW)
uint64_t hb_pow_get_global_work(const heartbeat_pow_context* hb) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
uint64_t hb_acc_pow_get_global_work(const heartbeat_acc_pow_context* hb) {
#else
uint64_t hb_get_global_work(const heartbeat_context* hb) {
#endif
  return hb == NULL ? 0 : hb->wd.global;
}

#if defined(HEARTBEAT_MODE_ACC)
uint64_t hb_acc_get_window_work(const heartbeat_acc_context* hb) {
#elif defined(HEARTBEAT_MODE_POW)
uint64_t hb_pow_get_window_work(const heartbeat_pow_context* hb) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
uint64_t hb_acc_pow_get_window_work(const heartbeat_acc_pow_context* hb) {
#else
uint64_t hb_get_window_work(const heartbeat_context* hb) {
#endif
  return hb == NULL ? 0 : hb->wd.window;
}

#if defined(HEARTBEAT_MODE_ACC)
double hb_acc_get_global_perf(const heartbeat_acc_context* hb) {
#elif defined(HEARTBEAT_MODE_POW)
double hb_pow_get_global_perf(const heartbeat_pow_context* hb) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
double hb_acc_pow_get_global_perf(const heartbeat_acc_pow_context* hb) {
#else
double hb_get_global_perf(const heartbeat_context* hb) {
#endif
  return hb == NULL ? 0.0 : hb->window_buffer[hb->ws.read_index].perf.global;
}

#if defined(HEARTBEAT_MODE_ACC)
double hb_acc_get_window_perf(const heartbeat_acc_context* hb) {
#elif defined(HEARTBEAT_MODE_POW)
double hb_pow_get_window_perf(const heartbeat_pow_context* hb) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
double hb_acc_pow_get_window_perf(const heartbeat_acc_pow_context* hb) {
#else
double hb_get_window_perf(const heartbeat_context* hb) {
#endif
  return hb == NULL ? 0.0 : hb->window_buffer[hb->ws.read_index].perf.window;
}

#if defined(HEARTBEAT_MODE_ACC)
double hb_acc_get_instant_perf(const heartbeat_acc_context* hb) {
#elif defined(HEARTBEAT_MODE_POW)
double hb_pow_get_instant_perf(const heartbeat_pow_context* hb) {
#elif defined(HEARTBEAT_MODE_ACC_POW)
double hb_acc_pow_get_instant_perf(const heartbeat_acc_pow_context* hb) {
#else
double hb_get_instant_perf(const heartbeat_context* hb) {
#endif
  return hb == NULL ? 0.0 : hb->window_buffer[hb->ws.read_index].perf.instant;
}
