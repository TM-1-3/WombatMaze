#ifndef __TIMER_H
#define __TIMER_H
#include <lcom/lcf.h>
#include "io/video/video.h"
#include <stdint.h>

/**
 * @brief Sets the frequency of the timer
 * @param timer Timer number
 * @param freq Desired frequency 
 * @return 0 on success, non-zero otherwise
 */
int (timer_set_frequency)(uint8_t timer, uint32_t freq);

/**
 * @brief Subscribes to timer interrupts
 * @param bit_no Address to store bit mask
 * @return 0 on success, non-zero otherwise
 */
int (timer_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from timer interrupts
 * @return 0 on success, non-zero otherwise
 */
int (timer_unsubscribe_int)();

/**
 * @brief Timer interrupt handler
 */
void (timer_int_handler)();

/**
 * @brief Gets the configuration of the timer
 * @param timer Timer number
 * @param st Pointer to where status byte will be stored
 * @return 0 on success, non-zero otherwise
 */
int (timer_get_conf)(uint8_t timer, uint8_t *st);

/**
 * @brief Displays timer configuration
 * @param timer Timer number
 * @param st Status byte
 * @param field Field of the configuration to display
 * @return 0 on success, non-zero otherwise
 */
int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field);

#endif // __TIMER_H
