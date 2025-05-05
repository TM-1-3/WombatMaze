#include "keyboard.h"

// Setup
int keyboard_hook_id = 1;
uint8_t scanCode = 0;

// Subscribes to keyboard interrupts
int (keyboard_subscribe_int)(uint8_t *bit_no){

    // Check null pointer
    if (bit_no == NULL){
        return 1;
    }

    // Set bitmask for the IRQ
    *bit_no = BIT(keyboard_hook_id);

    // Request access 
    if (sys_irqsetpolicy(IRQ_KEYBOARD, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id) != 0){
        return 1;
    }
    return 0;
}

// Unsubscribes from keyboard interrupts
int (keyboard_unsubscribe_int)(){

    // Remove the interrupt policy 
    if (sys_irqrmpolicy(&keyboard_hook_id) != 0){
        return 1;
    }
    return 0;
}

// Keyboard interrupt handler
void (kbc_ih)(){

    // Read data
    if (read_KBC_data(KBC_OB, &scanCode, 0) != 0){
        printf("Error reading KBC data");
    }
}

// Restores the keyboard interrupt
int (keyboard_restore_int)(){
    uint8_t commandByte;
    
    // Request to read
    if (write_KBC_data(KBC_CR, KBC_READ) != 0){
        return 1;
    }

    // Read the byte
    if (read_KBC_data(KBC_OB, &commandByte, 0) != 0){
        return 1;
    }
    commandByte |= ENABLE_INT;

    // Send the command
    if (write_KBC_data(KBC_CR, KBC_WRITE) != 0){
        return 1;
    }

    // Write the command
    if (write_KBC_data(KBC_IB, commandByte) != 0){
        return 1;
    }
    return 0;
}
