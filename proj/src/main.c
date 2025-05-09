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
#include "game/dingoe.h"
#include "game/maze.h"
#include "../assets/wombat/wombat_moving_4.xpm"
#include "../assets/dingoe/dingoe_moving_4.xpm"
#include "../assets/maze/maze_1.xpm"

// Global variable
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
    if (timer_subscribe_int(&timer_irq_set) != 0) {
        printf("Error: Failed to subscribe timer interrupts.\n");
        return 1;
    }
    if (keyboard_subscribe_int(&keyboard_irq_set) != 0) {
        printf("Error: Failed to subscribe keyboard interrupts.\n");
        return 1;
    }

    // Initialize video mode and framebuffer
    if (build_frame_buffer(DIRECT_600) != 0) {
        printf("Error: Failed to build framebuffer.\n");
        return 1;
    }
    if (set_mode_graphic(DIRECT_600) != 0) {
        printf("Error: Failed to set graphics mode.\n");
        return 1;
    }

    // Load Maze sprite
    Maze* maze = loadMaze(0, 0, (xpm_map_t)maze_1);
    if (maze == NULL) {
        printf("Error: Failed to load maze sprite.\n");
        return 1;
    }
    if (drawMaze(maze) != 0) {
        printf("Error: Failed to draw maze.\n");
        return 1;
    }

    // Load Wombat sprite
    Wombat* wombat = loadWombat(0, 0, (xpm_map_t)wombat_moving_4);
    if (wombat == NULL) {
        printf("Error: Failed to load wombat sprite.\n");
        return 1;
    }
    if (drawWombat(wombat) != 0) {
        printf("Error: Failed to draw wombat.\n");
        return 1;
    }
    int moveDirection = 0;

    // Load Dingoe sprite
    Dingoe* dingoe = loadDingoe(200, 400, (xpm_map_t)dingoe_moving_4);
    if (dingoe == NULL) {
        printf("Error: Failed to load dingoe sprite.\n");
        return 1;
    }
    if (drawDingoe(dingoe) != 0) {
        printf("Error: Failed to draw dingoe.\n");
        return 1;
    }
    int seeDirection = 0;

    // Main loop to process events until ESC is pressed
    while (scanCode != BREAK_ESC) {
        if (driver_receive(ANY, &msg, &ipc_status) != 0) {
            printf("Warning: driver_receive() failed.\n");
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
                        if (clear_screen() != 0) {
                            printf("Error: Failed to clear screen.\n");
                            return 1;
                        }

                        // Draw the maze
                        if (drawMaze(maze) != 0) {
                            printf("Error: Failed to draw maze.\n");
                            return 1;
                        }

                        // Draw wombat
                        if (moveDirection != 0) {
                            moveWombat(wombat, moveDirection, maze); 
                        }
                        if (drawWombat(wombat) != 0) {
                            printf("Error: Failed to draw wombat after move.\n");
                            return 1;
                        }

                        // Check if it sees wombat
                        int newDirection = seeWombat(dingoe, wombat, maze);
                        if (newDirection != 0) {
                            seeDirection = newDirection;
                        }

                        // Draw dingoe
                        if (seeDirection != 0) {
                            moveDingoe(dingoe, seeDirection, maze);
                        }
                        if (drawDingoe(dingoe) != 0) {
                            printf("Error: Failed to draw dingoe after move.\n");
                            return 1;
                        }

                        // Check collision
                        if (check_collision(dingoe, wombat)) {
                            printf("💥 Game Over! Wombat got caught!\n");
                            break;
                        }
                        
                        // Swap buffers
                        if (swap_buffers() != 0) {
                            printf("Error: Failed to swap buffers.\n");
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
        doubleBuffer = NULL;
    }

    // Exit graphical mode
    if (vg_exit() != 0) {
        printf("Error: Failed to exit graphics mode.\n");
        return 1;
    }
    return 0;
}
