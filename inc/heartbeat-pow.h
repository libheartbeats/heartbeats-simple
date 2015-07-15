/**
 * An extension of the capabilities in "heartbeat.h" to include monitoring
 * system power/energy usage.
 *
 * @author Connor Imes
 */
#ifndef _HEARTBEAT_POW_H_
#define _HEARTBEAT_POW_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "heartbeat-pow-types.h"
#include "heartbeat.h"

/**
 * Registers a heartbeat.
 *
 * @param hb
 * @param user_tag
 * @param work
 * @param start_time (ns)
 * @param end_time (ns)
 * @param start_energy (uJ)
 * @param end_energy (uJ)
 */
void heartbeat_pow(heartbeat_t* hb,
                   uint64_t user_tag,
                   uint64_t work,
                   uint64_t start_time,
                   uint64_t end_time,
                   uint64_t start_energy,
                   uint64_t end_energy);

/**
 * Get the total energy for the life of this heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the total energy (uint64_t)
 */
uint64_t hb_get_global_energy(const heartbeat_t* hb);

/**
 * Get the current window energy for this heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the window energy (uint64_t)
 */
uint64_t hb_get_window_energy(const heartbeat_t* hb);

/**
 * Returns the power over the life of the entire application
 *
 * @param hb pointer to heartbeat_t
 * @return the power (double) over the entire life of the application
 */
double hb_get_global_power(const heartbeat_t* hb);

/**
 * Returns the power over the last window (as specified to init)
 * heartbeats
 *
 * @param hb pointer to heartbeat_t
 * @return the power (double) over the last window
 */
double hb_get_window_power(const heartbeat_t* hb);

/**
 * Returns the power for the last heartbeat.
 *
 * @param hb pointer to heartbeat_t
 * @return the power (double) for the last heartbeat
 */
double hb_get_instant_power(const heartbeat_t* hb);

/**
 * Returns the energy recorded in this record.
 *
 * @param hbr
 * @return the energy (uint64_t)
 */
uint64_t hbr_get_energy(const heartbeat_record_t* hbr);

/**
 * Returns the global power recorded in this record.
 *
 * @param hbr
 * @return the global power (double)
 */
double hbr_get_global_power(const heartbeat_record_t* hbr);

/**
 * Returns the window power recorded in this record.
 *
 * @param hbr
 * @return the window power (double)
 */
double hbr_get_window_power(const heartbeat_record_t* hbr);

/**
 * Returns the instant power recorded in this record.
 *
 * @param hbr
 * @return the instant power (double)
 */
double hbr_get_instant_power(const heartbeat_record_t* hbr);

#ifdef __cplusplus
}
#endif

#endif
