#ifndef _LCOM_MOUSE_H_
#define _LCOM_MOUSE_H_
#include <minix/sysutil.h>
#include <lcom/lcf.h>
#include "../i8042.h"
#include "../KBC.h"

/**
 * @brief Subscribes to mouse interrupts
 * @param bit_no Address to store the bit the mask
 * @return 0 on success, non-zero otherwise
 */
int (mouse_subscribe_int)(uint8_t *bit_no);

/**
 * @brief Unsubscribes from mouse interrupts
 * @return 0 on success, non-zero otherwise
 */
int (mouse_unsubscribe_int)();

/**
 * @brief Mouse interrupt handler
 */
void (mouse_ih)();

/**
 * @brief Sends a command byte to the mouse
 * @param command Command byte to send
 * @return 0 on success, non-zero otherwise
 */
int (mouse_write_data)(uint8_t command);


/**
 * @brief Synchronizes the mouse bytes
 */
void (mouse_sync_bytes)();

/**
 * @brief Builds the full mouse packet
 */
void (mouse_build_packet)();

#endif
