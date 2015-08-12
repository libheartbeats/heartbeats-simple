/**
 * Power utility functions.
 *
 * @author Connor Imes
 */

#include <inttypes.h>
#include <stdlib.h>

/* Determine which heartbeat implementation to use */
#if defined(HEARTBEAT_MODE_ACC_POW)
#include "heartbeat-acc-pow.h"
#else
#include "heartbeat-pow.h"
#endif

#if defined(HEARTBEAT_MODE_ACC_POW)
uint64_t hb_acc_pow_get_global_energy(const heartbeat_acc_pow_context* hb) {
#else
uint64_t hb_pow_get_global_energy(const heartbeat_pow_context* hb) {
#endif
  return hb == NULL ? 0 : hb->ed.global;
}

#if defined(HEARTBEAT_MODE_ACC_POW)
uint64_t hb_acc_pow_get_window_energy(const heartbeat_acc_pow_context* hb) {
#else
uint64_t hb_pow_get_window_energy(const heartbeat_pow_context* hb) {
#endif
  return hb == NULL ? 0 : hb->ed.window;
}

#if defined(HEARTBEAT_MODE_ACC_POW)
double hb_acc_pow_get_global_power(const heartbeat_acc_pow_context* hb) {
#else
double hb_pow_get_global_power(const heartbeat_pow_context* hb) {
#endif
  return hb == NULL ? 0.0 : hb->window_buffer[hb->ws.read_index].pwr.global;
}

#if defined(HEARTBEAT_MODE_ACC_POW)
double hb_acc_pow_get_window_power(const heartbeat_acc_pow_context* hb) {
#else
double hb_pow_get_window_power(const heartbeat_pow_context* hb) {
#endif
  return hb == NULL ? 0.0 : hb->window_buffer[hb->ws.read_index].pwr.window;
}

#if defined(HEARTBEAT_MODE_ACC_POW)
double hb_acc_pow_get_instant_power(const heartbeat_acc_pow_context* hb) {
#else
double hb_pow_get_instant_power(const heartbeat_pow_context* hb) {
#endif
  return hb == NULL ? 0.0 : hb->window_buffer[hb->ws.read_index].pwr.instant;
}
