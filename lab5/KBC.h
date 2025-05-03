#ifndef _LCOM_KBC_H_
#define _LCOM_KBC_H_

#include <minix/sysutil.h>
#include <lcom/lcf.h>

#include "i8042.h"

/**
 * @brief Reads data from the KBC
 * @param port Port to read from
 * @param output Pointer to where the read data will be stored
 * @param mouse 1 if reading from mouse, 0 if from keyboard
 * @return 0 on success, non-zero otherwise
 */
int (read_KBC_data)(uint8_t port, uint8_t *output, uint8_t mouse);

/**
 * @brief Writes data to the KBC
 * @param port Port to write to
 * @param commandByte Byte to write
 * @return 0 on success, non-zero otherwise
 */
int (write_KBC_data)(uint8_t port, uint8_t commandByte);

#endif
