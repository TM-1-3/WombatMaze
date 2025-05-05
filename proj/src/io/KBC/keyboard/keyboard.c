#include "keyboard.h"
#include <stdio.h>  // Include for printf

// Setup
int keyboard_hook_id = 1;
uint8_t scanCode = 0;

// Subscribes to keyboard interrupts
int (keyboard_subscribe_int)(uint8_t *bit_no) {

    // Check null pointer
    if (bit_no == NULL) {
        printf("Error: Null pointer received for bit_no.\n");
        return 1;
    }

    // Set bitmask for the IRQ
    *bit_no = BIT(keyboard_hook_id);

    // Request exclusive and re-enabled interrupt policy
    if (sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != 0) {
        printf("Error: Failed to subscribe to keyboard interrupt (IRQ_KEYBOARD).\n");
        return 1;
    }
    return 0;
}

// Unsubscribes from keyboard interrupts
int (keyboard_unsubscribe_int)() {

    // Remove the interrupt policy
    if (sys_irqrmpolicy(&keyboard_hook_id) != 0) {
        printf("Error: Failed to unsubscribe from keyboard interrupt (IRQ_KEYBOARD).\n");
        return 1;
    }
    return 0;
}

// Keyboard interrupt handler
void (kbc_ih)() {
    uint8_t status;

    // Read KBC status
    if (util_sys_inb(KBC_SR, &status) != 0) {
        printf("Error: Failed to read KBC status register.\n");
        return;
    }

    // Check for errors and if output buffer is full
    if ((status & STAT_OBF) && !(status & (STAT_PARITY | STAT_TIMEOUT))) {
        if (util_sys_inb(KBC_OB, &scanCode) != 0) {
            printf("Error: Failed to read scanCode from KBC output buffer.\n");
        }
    } else {
        if (status & (STAT_PARITY | STAT_TIMEOUT)) {
            printf("Error: KBC error - Parity or Timeout detected.\n");
        }
    }
}

// Restores the keyboard interrupt
int (keyboard_restore_int)() {
    uint8_t commandByte;

    // Request to read command byte
    if (write_KBC_data(KBC_CR, KBC_READ) != 0) {
        printf("Error: Failed to send read command to KBC.\n");
        return 1;
    }

    // Read the current command byte
    if (read_KBC_data(KBC_OB, &commandByte, 0) != 0) {
        printf("Error: Failed to read command byte from KBC.\n");
        return 1;
    }

    // Enable interrupts
    commandByte |= ENABLE_INT;

    // Send the write command
    if (write_KBC_data(KBC_CR, KBC_WRITE) != 0) {
        printf("Error: Failed to send write command to KBC.\n");
        return 1;
    }

    // Write the updated command byte
    if (write_KBC_data(KBC_IB, commandByte) != 0) {
        printf("Error: Failed to write updated command byte to KBC.\n");
        return 1;
    }
    return 0;
}
