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
#include "game/obstacle.h"
#include "game/cursor.h"
#include "../assets/wombat/wombat_moving_1.xpm"
#include "../assets/wombat/wombat_moving_4.xpm"
#include "../assets/dingoe/dingoe_moving_4.xpm"
#include "../assets/maze/maze_1.xpm"
#include "../assets/cursor/cursor.xpm"
#include "../assets/obstacle/obstacle1.xpm"


// Global variable
volatile extern uint8_t scanCode;
extern uint32_t timerCounter;
extern struct packet mousePacket;
extern uint8_t byteIndex;

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
    #define MAX_OBSTACLES 2
    Obstacle* obstacles[MAX_OBSTACLES];
    uint8_t timer_irq_set, keyboard_irq_set, mouse_irq_set;
    int ipc_status;
    message msg;

    // Enable stream mode and data report in the mouse
    if (mouse_write_data(ENABLE_STREAM_MODE) != 0 || mouse_write_data(ENABLE_DATA_REPORT) != 0) {
        printf("Error: Failed to initialize mouse.\n");
        return 1;
    }

    // Set frequency
    if (timer_set_frequency(0, 30)!=0){
        return 1;
    }

    // Subscribe to IRQs
    if (timer_subscribe_int(&timer_irq_set) != 0) {
        printf("Error: Failed to subscribe timer interrupts.\n");
        return 1;
    }
    if (keyboard_subscribe_int(&keyboard_irq_set) != 0) {
        printf("Error: Failed to subscribe keyboard interrupts.\n");
        return 1;
    }
    if (mouse_subscribe_int(&mouse_irq_set)!=0) {
        printf("Error: Failed to subscribe mouse interrupts.\n");
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

    // Load digit sprites
    Sprite **digits = loadDigitSprites();
    if (digits == NULL) {
        printf("Error: Failed to load digit sprites.\n");
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

    // Load Wombats sprite
    Sprite* wombatSprite1 = loadSprite((xpm_map_t)wombat_moving_1);
    Sprite* wombatSprite2 = loadSprite((xpm_map_t)wombat_moving_4);
    Wombat* currentWombat = loadWombat(0, 0, wombatSprite1);

    // Draw wombat
    if (drawWombat(currentWombat) != 0) {
        printf("Error: Failed to draw wombat.\n");
        return 1;
    }
    int moveDirection = 0;

    // Load Obstacle sprites
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        uint16_t x = 200 + i * 50; // Podes ajustar posições
        uint16_t y = 500;
        obstacles[i] = load_obstacle(x, y, (xpm_map_t)obstacle_xpm);
        if (obstacles[i] == NULL) {
            printf("Error: Failed to load obstacle %d\n", i);
            return 1;
        }
    }

    // Toggle wombats
    int wombat_counter = 0;
    bool wombat_toggle = false;

    // Load Dingoe sprite
    Sprite* dingoeSprite1 = loadSprite((xpm_map_t)wombat_moving_1);
    Sprite* dingoeSprite2 = loadSprite((xpm_map_t)wombat_moving_4);
    Dingoe* currentDingoe = loadDingoe(200, 100, dingoeSprite1);

    // Draw dingoe
    if (drawDingoe(currentDingoe) != 0) {
        printf("Error: Failed to draw dingoe.\n");
        return 1;
    }
    int seeDirection = 0;

    // Toggle dingoes
    int dingoe_counter = 0;
    bool dingoe_toggle = false;

    // Cursor
    Cursor* cursor = loadCursor(5, 5, (xpm_map_t)cursor_xpm);
    if (cursor == NULL){
        printf("Error: Failed to load cursor sprite.\n");
        return 1;
    }
    bool gameOver=false;


    // Main loop to process events until ESC is pressed
    while (scanCode != BREAK_ESC && !gameOver) {
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

                    // Check if it´s a mouse interrupt
                    if (msg.m_notify.interrupts & mouse_irq_set){
                        mouse_ih();
                        mouse_sync_bytes();
                        if (byteIndex==0){
                            mouse_build_packet();
                            if (mousePacket.x_ov || mousePacket.y_ov){
                                break;
                            }
                            int mouseX = getCursorX(cursor) + mousePacket.delta_x;
                            int mouseY = getCursorY(cursor) - mousePacket.delta_y;

                            if (mouseX < 0 && !mousePacket.x_ov){
                                mouseX = 0;
                            }
                            else if (mouseX > SCREEN_WIDTH-cursor->cursorSprite->width && !mousePacket.x_ov) {
                                mouseX = SCREEN_WIDTH - cursor->cursorSprite->width;
                            }
                            if (mouseY < 0 && !mousePacket.y_ov) {
                                mouseY = 0;
                            }
                            else if (mouseY > SCREEN_HEIGHT-cursor->cursorSprite->height && !mousePacket.y_ov) {
                                mouseY = SCREEN_HEIGHT - cursor->cursorSprite->height;
                            }
                            setCursorX(cursor, mouseX);
                            setCursorY(cursor, mouseY);
                            for (int i = 0; i < MAX_OBSTACLES; i++) {
                                if(obstacles[i]->active && is_wombat_near_obstacle(currentWombat, obstacles[i])){
                                    update_obstacle_with_cursor(obstacles[i], cursor, &mousePacket);
                                }
                            }
                        }   
                    }

                    // Check if it's a timer interrupt
                    if (msg.m_notify.interrupts & timer_irq_set) {
                        
                        // Clear the screen
                        if (clear_screen() != 0) {
                            printf("Error: Failed to clear screen.\n");
                            return 1;
                        }
                        timer_int_handler();
                        int elapsed_seconds = timerCounter / 60;
                        int time_left = 100 - elapsed_seconds;
                        
                        // Draw time left
                        if (drawNumber(digits, time_left, 790, 10) != 0) {
                            printf("Error: Failed to draw time.\n");
                            return 1;
                        }           
                                                
                        // Time's up game over
                        if (time_left <= 0) {
                            time_left = 0;
                            printf("Time's up! Game Over\n");
                            gameOver=true;
                        }

                        // Draw the maze
                        if (drawMaze(maze) != 0) {
                            printf("Error: Failed to draw maze.\n");
                            return 1;
                        }

                        // Move wombat
                        if (moveDirection != 0) {
                            wombat_counter++;
                            if (wombat_counter % 5 == 0) {
                                wombat_toggle = !wombat_toggle;
                                currentWombat->wombatSprite = wombat_toggle ? wombatSprite2 : wombatSprite1;
                            }
                            moveWombat(currentWombat, moveDirection, maze, obstacles, MAX_OBSTACLES);
                        }

                        // Draw wombat
                        if (drawWombat(currentWombat) != 0) {
                            printf("Error: Failed to draw dingoe after move.\n");
                            return 1;
                        }

                        // Draw obtacles
                        for (int i = 0; i < MAX_OBSTACLES; i++) {
                            if (obstacles[i]->active) {
                                if (draw_obstacle(obstacles[i]) != 0) {
                                    printf("Error: Failed to draw obstacle %d.\n", i);
                                    return 1;
                                }
                            }
                        }

                        // Draw cursor
                        if (drawCursor(cursor)!=0){
                            printf("Error: Failed to draw cursor.\n");
                            return 1;
                        }

                        // Check if it sees wombat
                        int newDirection = seeWombat(currentDingoe, currentWombat, maze);
                        if (newDirection != 0) {
                            seeDirection = newDirection;
                        }

                        // Move dingoe
                        if (seeDirection != 0) {
                            dingoe_counter++;
                            if (dingoe_counter % 5 == 0) {
                                dingoe_toggle = !dingoe_toggle;
                                currentDingoe->dingoeSprite = dingoe_toggle ? dingoeSprite2 : dingoeSprite1;
                            }
                            moveDingoe(currentDingoe, seeDirection, maze);
                        }

                        // Draw dingoe
                        if (drawDingoe(currentDingoe) != 0) {
                            printf("Error: Failed to draw dingoe after move.\n");
                            return 1;
                        }

                        // Check collision
                        if (check_collision(currentDingoe, currentWombat)) {
                            printf("💥 Game Over! Wombat got caught!\n");
                            gameOver=true;
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

    // Free obstales
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i] != NULL) {
            if (obstacles[i]->obstacleSprite != NULL) {
                free(obstacles[i]->obstacleSprite);
            }
            free(obstacles[i]);
        }
    }

    // Free digits
    for (int i = 0; i < 10; i++) {
        if (digits[i] != NULL) {
            free(digits[i]->colors);
            free(digits[i]);
        }
    }
    free(digits);

    // Exit graphical mode
    if (vg_exit() != 0) {
        printf("Error: Failed to exit graphics mode.\n");
        return 1;
    }

    if (timer_unsubscribe_int()!=0){
        return 1;
    }
    if (keyboard_unsubscribe_int()!=0){
        return 1;
    }
    if (mouse_unsubscribe_int()!=0){
        return 1;
    }
    if (mouse_write_data(DISABLE_DATA_REPORT)!=0){
        return 1;
    }

    return 0;
}
