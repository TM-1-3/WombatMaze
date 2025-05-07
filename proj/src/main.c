#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include "io/video/video.h"
#include "io/timer/timer.h"
#include "io/KBC/KBC.h"
#include "io/KBC/keyboard/keyboard.h"
#include "io/KBC/mouse/mouse.h"
#include "display/sprite.h"
#include "game/wombat.h"
#include "assets/wombat/wombat_moving_4.xpm"

// Global variable for scanCode handling
volatile extern uint8_t scanCode;

/**
 * @brief Starts the program
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 on success, non-zero otherwise
 */
int (main)(int argc, char *argv[]) {
    
    // Set the language for LCF messages (EN-US or PT-PT)
    lcf_set_language("EN-US");

    // Enable logging function invocations
    lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

    // Enable saving printf output to a file
    lcf_log_output("/home/lcom/labs/proj/output.txt");

    // Hand over control to LCF to handle the program logic
    if (lcf_start(argc, argv)) {
        return 1;
    }

    // LCF cleanup after the program ends
    lcf_cleanup();
    return 0;
}

/**
 * @brief Main game loop
 * @param argc Argument count 
 * @param argv Argument vector 
 * @return 0 on success, non-zero otherwise
 */
int (proj_main_loop)(int argc, char *argv[]) {
    uint8_t timer_irq_set, keyboard_irq_set;
    int ipc_status;
    message msg;

    // Subscribe to IRQs
    if (timer_subscribe_int(&timer_irq_set) != 0 || keyboard_subscribe_int(&keyboard_irq_set) != 0) {
        return 1;
    }

    // Initialize video mode and framebuffer
    if (build_frame_buffer(DIRECT_600) != 0 || set_mode_graphic(DIRECT_600) != 0) {
        return 1;
    }

    // Load Wombat sprite
    Wombat* wombat = loadWombat(10, 10, (xpm_map_t)wombat_moving_4);
    if (drawWombat(wombat) != 0) {
        return 1;
    }
    int moveDirection = 0;

    // Main loop to process events until ESC is pressed
    while (scanCode != BREAK_ESC) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            continue;  
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:

                    // Check if it's a keyboard interrupt
                    if (msg.m_notify.interrupts & keyboard_irq_set) {
                        kbc_ih();
                        moveDirection = moveHandler(scanCode);
                    }

                    // Check if it's a timer interrupt
                    if (msg.m_notify.interrupts & timer_irq_set) {

                        // Clear the screen
                        clear_screen();

                        // Draw a background or other element
                        if (draw_rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK) != 0) {
                            return 1;
                        }

                        // If there's a move direction, move Wombat and draw it
                        if (moveDirection != 0) {
                            moveWombat(wombat, moveDirection);
                        }
                        if (drawWombat(wombat) != 0) {
                            return 1;
                        }

                        // Double buffering
                        if (swap_buffers() != 0) {
                            return 1;
                        }
                    }
                    break;
            }
        }
    }

    // Free double buffer
    if (doubleBuffer != NULL) {
        free(doubleBuffer);
    }

    // Exit graphical mode
    if (vg_exit() != 0) {
        return 1;
    }
    return 0;
}
