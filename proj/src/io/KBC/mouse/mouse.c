#include "mouse.h"
#include <stdio.h>  // Include for printf

// Setup
int mouse_hook_id = 3;
uint8_t mouseByte;
uint8_t byteIndex = 0;
uint8_t mouseBytes[3];
struct packet mousePacket;

// Subscribes mouse interrupts
int (mouse_subscribe_int)(uint8_t *bit_no){

    // Check if possible
    if (bit_no == NULL){
        printf("Error: Invalid pointer to bit_no.\n");
        return 1;
    }

    // Set bit mask
    *bit_no = BIT(mouse_hook_id);

    // Subscribe to IRQ12
    if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id) != 0){
        printf("Error: Failed to subscribe to mouse IRQ12.\n");
        return 1;
    }
    return 0;
}

// Unsubscribes mouse interrupts
int (mouse_unsubscribe_int)(){

    // Unsubscribe to IRQ12
    if (sys_irqrmpolicy(&mouse_hook_id) != 0){
        printf("Error: Failed to unsubscribe from mouse IRQ12.\n");
        return 1;
    }
    return 0;
}

// Mouse interrupt handler
void (mouse_ih)(){

    // Reads byte from mouse
    if (read_KBC_data(KBC_OB, &mouseByte, 1) != 0){
        printf("Error: Failed to read data from KBC.\n");
    }
}

// Writes a command to the mouse
int (mouse_write_data)(uint8_t command){
    uint8_t attempts = MAX_ATTEMPTS;
    uint8_t responseMouse;

    // The loop
    while (attempts--) {

        // Tell KBC we want to write to mouse
        if (write_KBC_data(KBC_CR, MOUSE_WRITE_CMD) != 0) {
            printf("Error: Failed to send write command to KBC.\n");
            return 1;
        }

        // Write command to mouse
        if (write_KBC_data(KBC_IB, command) != 0) {
            printf("Error: Failed to write command 0x%X to KBC.\n", command);
            return 1;
        }

        // Wait before checking response
        tickdelay(micros_to_ticks(WAIT_KBC));

        // Read mouse response
        if (util_sys_inb(KBC_OB, &responseMouse) != 0) {
            printf("Error: Failed to read mouse response from KBC.\n");
            return 1;
        }

        // If ACK received, success
        if (responseMouse == MOUSE_ACK) return 0;
        printf("Error: Mouse did not acknowledge command 0x%X. Response: 0x%X.\n", command, responseMouse);
    }
    printf("Error: Mouse failed to acknowledge command 0x%X after %d attempts.\n", command, MAX_ATTEMPTS);
    return 1;
}

// Synchronizes the bytes into 3-byte
void (mouse_sync_bytes)(){
    if ((mouseByte & MOUSE_SYNC) && byteIndex == 0){

        // First byte must have bit 3 set
        mouseBytes[byteIndex++] = mouseByte;
    }
    else if (byteIndex > 0){

        // Add 2nd and 3rd byte
        mouseBytes[byteIndex++] = mouseByte;
    }

    // Reset index after full packet
    if (byteIndex == 3) {
        byteIndex = 0;
    }
}

// Parses the 3-byte into a struct
void (mouse_build_packet)(){

    // Copy bytes
    for (int i = 0; i < 3; i++){
        mousePacket.bytes[i] = mouseBytes[i];
    }

    // Button states
    mousePacket.lb = mouseBytes[0] & MOUSE_LB;
    mousePacket.rb = mouseBytes[0] & MOUSE_RB;
    mousePacket.mb = mouseBytes[0] & MOUSE_MB;

    // Overflow flags
    mousePacket.x_ov = mouseBytes[0] & MOUSE_XOVER;
    mousePacket.y_ov = mouseBytes[0] & MOUSE_YOVER;

    // Convert movement
    mousePacket.delta_x = (mouseBytes[0] & MOUSE_XSIGNAL) ? (0xFF00 | mouseBytes[1]) : mouseBytes[1];
    mousePacket.delta_y = (mouseBytes[0] & MOUSE_YSIGNAL) ? (0xFF00 | mouseBytes[2]) : mouseBytes[2];
}
