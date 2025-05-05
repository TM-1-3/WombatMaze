#include "KBC.h"

// Read data from the KBC
int (read_KBC_data)(uint8_t port, uint8_t *output, uint8_t mouse){

    // Setup
    uint8_t stat, data;
    uint8_t attempts = 10;

    // The loop
    while (attempts){

        // Read status register
        if (util_sys_inb(KBC_SR, &stat) != 0){
            return 1;
        }

        // Check OBF
        if ((stat & STAT_OBF) != 0){

            // Read the data byte
            if (util_sys_inb(port, &data) != 0){
                return 1;
            }

            // Check if it's from mouse or keyboard
            if (mouse && !(stat & STAT_AUX)) {
                return 1;
            }
            if (!mouse && (stat & STAT_AUX)) {
                return 1;
            }

            // Check for errors
            if ((stat & (STAT_PARITY | STAT_TIMEOUT)) == 0){
                *output = data;
                return 0;
            }
            else{
                return 1;
            }
        }

        // Delay and retry
        tickdelay(micros_to_ticks(WAIT_KBC));
        attempts--;
    }
    return 1;
}

// Write data to the KBC
int (write_KBC_data)(uint8_t port, uint8_t commandByte){

    // Setup
    uint8_t stat;
    uint8_t attempts = MAX_ATTEMPTS;

    // The loop
    while (attempts){

        // Read status register
        if (util_sys_inb(KBC_SR, &stat) != 0){
            return 1;
        }

        // Check IBF
        if ((stat & STAT_IBF) == 0){

            // Write the data byte
            if (sys_outb(port, commandByte) != 0){
                return 1;
            }
            return 0;
        }

        // Delay and retry
        tickdelay(micros_to_ticks(WAIT_KBC));
        attempts--;
    }
    return 1;
}
