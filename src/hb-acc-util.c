/**
 * Accuracy utility functions.
 *
 * @author Connor Imes
 */

#include <inttypes.h>

/* Determine which heartbeat implementation to use */
#if defined(HEARTBEAT_MODE_ACC_POW)
#include "heartbeat-acc-pow.h"
#else
#include "heartbeat-acc.h"
#endif

#if defined(HEARTBEAT_MODE_ACC_POW)
uint64_t hb_acc_pow_get_global_accuracy(const heartbeat_acc_pow_context* hb) {
#else
uint64_t hb_acc_get_global_accuracy(const heartbeat_acc_context* hb) {
#endif
  return hb->ad.global;
}

#if defined(HEARTBEAT_MODE_ACC_POW)
uint64_t hb_acc_pow_get_window_accuracy(const heartbeat_acc_pow_context* hb) {
#else
uint64_t hb_acc_get_window_accuracy(const heartbeat_acc_context* hb) {
#endif
  return hb->ad.window;
}

#if defined(HEARTBEAT_MODE_ACC_POW)
double hb_acc_pow_get_global_accuracy_rate(const heartbeat_acc_pow_context* hb) {
#else
double hb_acc_get_global_accuracy_rate(const heartbeat_acc_context* hb) {
#endif
  return hb->window_buffer[hb->ws.read_index].acc.global;
}

#if defined(HEARTBEAT_MODE_ACC_POW)
double hb_acc_pow_get_window_accuracy_rate(const heartbeat_acc_pow_context* hb) {
#else
double hb_acc_get_window_accuracy_rate(const heartbeat_acc_context* hb) {
#endif
  return hb->window_buffer[hb->ws.read_index].acc.window;
}

#if defined(HEARTBEAT_MODE_ACC_POW)
double hb_acc_pow_get_instant_accuracy_rate(const heartbeat_acc_pow_context* hb) {
#else
double hb_acc_get_instant_accuracy_rate(const heartbeat_acc_context* hb) {
#endif
  return hb->window_buffer[hb->ws.read_index].acc.instant;
}
