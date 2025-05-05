#ifndef _LCOM_KEYBOARD_H_
#define _LCOM_KEYBOARD_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "i8042.h"
#include "KBC.h"

/**
 * @brief Subscribes keyboard interrupts
 * @param bit_no Address of memory to be set with the bit mask to identify the IRQ line
 * @return 0 on success, non-zero otherwise
 */
int (keyboard_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes keyboard interrupts
 * @return 0 on success, non-zero otherwise
 */
int (keyboard_unsubscribe_int)();

/**
 * @brief Keyboard interrupt handler
 */
void (kbc_ih)();


/**
 * @brief Restores keyboard interrupt configuration
 * @return 0 on success, non-zero otherwise
 */
int (keyboard_restore_int)();

#endif
