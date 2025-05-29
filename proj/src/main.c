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
#include "../assets/wombat/wombat_moving_2.xpm"
#include "../assets/wombat/wombat_attacking_1.xpm"
#include "../assets/wombat/wombat_attacking_2.xpm"
#include "../assets/dingoe/dingoe_moving_1.xpm"
#include "../assets/dingoe/dingoe_moving_2.xpm"
#include "game/menu.h"
#include "../assets/maze/map_forest.xpm"
#include "../assets/maze/map_space.xpm"
#include "../assets/maze/map_sand.xpm"
#include "../assets/maze/map_forest_bw.xpm"
#include "../assets/maze/map_space_bw.xpm"
#include "../assets/maze/map_sand_bw.xpm"
#include "game/game.h"
#include "../assets/cursor/cursor.xpm"
#include "../assets/obstacle/obstacle1.xpm"
#include "../../assets/menu/logo.xpm"
#include "../../assets/menu/backgroundMenu.xpm"
#include "../../assets/menu/cursorMenu.xpm"
#include "../../assets/menu/instructions.xpm"
#include "../../assets/screens/gameOver.xpm"

typedef enum { 
    MENU,
    INSTRUCTIONS,
    LEVEL,
    GAME_OVER,
    END
} State;

State state = MENU;

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
    MenuElement* menuBackground = loadMenuElement(0, 0, (xpm_map_t)background_xpm);
    MenuElement* logo = loadMenuElement(150, 30, (xpm_map_t)logo_xpm);
    MenuElement* menuCursor = loadMenuElement(160, 255, (xpm_map_t)menu_cursor_xpm);
    MenuElement* instructions = loadMenuElement(0, 0, (xpm_map_t)instructions_xpm);
    MenuElement* gameOver = loadMenuElement(0, 0, (xpm_map_t)game_over_xpm);

    // Load digit sprites
    Sprite **digits = loadDigitSprites();
    if (digits == NULL) {
        printf("Error: Failed to load digit sprites.\n");
        return 1;
    }

    // Load Maze sprite
    Maze* maze = loadMaze(0, 0, (xpm_map_t)map_forest);
    if (maze == NULL) {
        printf("Error: Failed to load maze sprite.\n");
        return 1;
    }
    Maze* maze_bw = loadMaze(0, 0, (xpm_map_t)map_forest_bw);
    if (maze == NULL) {
        printf("Error: Failed to load maze sprite.\n");
        return 1;
    }

    // Load Wombats sprite
    Sprite* wombatMoving1 = loadSprite((xpm_map_t)wombat_moving_1);
    Sprite* wombatMoving2 = loadSprite((xpm_map_t)wombat_moving_2);
    Sprite* wombatAttacking1 = loadSprite((xpm_map_t)wombat_attacking_1);
    Sprite* wombatAttacking2 = loadSprite((xpm_map_t)wombat_attacking_2);
    Wombat* currentWombat = loadWombat(50, 50, wombatMoving1);
    int moveDirection = 0;

    // Load Obstacle sprites
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        uint16_t x = 125 + i * 50; // Podes ajustar posições
        uint16_t y = 400;
        obstacles[i] = load_obstacle(x, y, (xpm_map_t)obstacle_xpm);
        if (obstacles[i] == NULL) {
            printf("Error: Failed to load obstacle %d\n", i);
            return 1;
        }
    }

    // Toggle moving wombats
    int wombatMoving_counter = 0;
    bool wombatMoving_toggle = false;

    // Toggle attacking wombats
    static int wombatAttacking_counter = 0;
    static bool wombatAttacking_toggle = false;

    // Toggle wombat
    bool isAttacking = false;

    // Load Dingoe sprite
    Sprite* dingoeMoving1 = loadSprite((xpm_map_t)dingoe_moving_1);
    Sprite* dingoeMoving2 = loadSprite((xpm_map_t)dingoe_moving_2);
    Dingoe* currentDingoe = loadDingoe(500, 50, dingoeMoving1);
    
    int seeDirection = 0;

    // Toggle moving dingoes
    int dingoeMoving_counter = 0;
    bool dingoeMoving_toggle = false;

    // Cursor
    Cursor* cursor = loadCursor(5, 5, (xpm_map_t)cursor_xpm);
    if (cursor == NULL){
        printf("Error: Failed to load cursor sprite.\n");
        return 1;
    }
    
    
    // Main loop to process events until ESC is pressed
    while (state != END) {
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
                        if (state == MENU) {
                            if (scanCode == MAKE_ENTER && getMenuElementY(menuCursor) == 255) {
                                timerCounter = 0;
                                reset_level(currentWombat, currentDingoe);
                                state = LEVEL;
                            }
                            else if (scanCode == MAKE_ENTER && getMenuElementY(menuCursor) == 335) {
                                state = INSTRUCTIONS;
                            }
                            else if (scanCode == MAKE_S) {
                                menuCursorDown(menuCursor);
                            }
                            else if (scanCode == MAKE_W) {
                                menuCursorUp(menuCursor);
                            }
                            else if (scanCode == MAKE_ENTER && getMenuElementY(menuCursor) == 405) {
                                state = END;
                            }
                        }
                        else if (state == INSTRUCTIONS) {
                            if (scanCode == MAKE_ESC) {
                                state = MENU;
                            }
                        }
                        else if (state == GAME_OVER) {
                            if (scanCode == MAKE_ESC) {
                                state = END;
                            }
                            else if (scanCode == MAKE_ENTER) {
                                state = MENU;
                            }
                        }
                        else if (state == LEVEL) {
                            moveDirection = moveHandler(scanCode);
                            if (scanCode == MAKE_ESC) {
                                state = MENU;
                            }
                        }
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

                            // Get out of menu
                            if (mousePacket.lb){
                                if (state == MENU) {
                                    state = LEVEL;
                                }
                            }

                            // Get coordinates
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

                            // Set coordinates
                            setCursorX(cursor, mouseX);
                            setCursorY(cursor, mouseY);

                            // Check for left button click
                            if (mousePacket.lb) {
                                isAttacking = true;

                                // Remove obstacles
                                for (int i = 0; i < MAX_OBSTACLES; i++) {
                                    if(obstacles[i]->active && is_wombat_near_obstacle(currentWombat, obstacles[i])){
                                        update_obstacle_with_cursor(obstacles[i], cursor);
                                    }
                                }
                            }        
                            else {
                                isAttacking = false;
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

                        if (state == MENU) {
                            drawMenu(logo, menuBackground, menuCursor);
                        }
                        else if (state == INSTRUCTIONS) {
                            drawMenuElement(instructions);
                        }
                        else if (state == GAME_OVER) {
                            drawMenuElement(gameOver);
                        }
                        else if (state == LEVEL) {
                        int elapsed_seconds = timerCounter / 60;
                        int time_left = 100 - elapsed_seconds; 
                                    
                        // Time's up game over
                        if (time_left <= 0) {
                            time_left = 0;
                            printf("Time's up! Game Over\n");
                            state = GAME_OVER;
                        }

                        // Draw the maze
                        if (drawMaze(maze) != 0) {
                            printf("Error: Failed to draw maze.\n");
                            return 1;
                        }

                        // Draw time left
                        if (drawNumber(digits, time_left, 790, 10) != 0) {
                            printf("Error: Failed to draw time.\n");
                            return 1;
                        }  

                        // Move wombat
                        if (moveDirection != 0) {
                            wombatMoving_counter++;
                            if (wombatMoving_counter % 5 == 0) {
                                wombatMoving_toggle = !wombatMoving_toggle;
                                currentWombat->wombatSprite = wombatMoving_toggle ? wombatMoving2 : wombatMoving1;
                            }
                            moveWombat(currentWombat, moveDirection, maze_bw, obstacles, MAX_OBSTACLES);
                        }

                        // Set attack
                        if (isAttacking){
                            wombatAttacking_counter++;
                            if (wombatAttacking_counter % 5 == 0) {
                                wombatAttacking_toggle = !wombatAttacking_toggle;
                                currentWombat->wombatSprite = wombatAttacking_toggle ? wombatAttacking2 : wombatAttacking1;
                            }
                        }

                        // Draw wombat
                        if (drawWombat(currentWombat) != 0) {
                            printf("Error: Failed to draw dingoe after move.\n");
                            return 1;
                        }

                        // Reset
                        if (!mousePacket.lb && moveDirection == 0) {
                            currentWombat->wombatSprite = wombatMoving1; 
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

                        // Check if it sees wombat
                        int newDirection = seeWombat(currentDingoe, currentWombat, maze_bw);
                        if (newDirection != 0) {
                            seeDirection = newDirection;
                        }

                        // Move dingoe
                        if (seeDirection != 0) {
                            dingoeMoving_counter++;
                            if (dingoeMoving_counter % 5 == 0) {
                                dingoeMoving_toggle = !dingoeMoving_toggle;
                                currentDingoe->dingoeSprite = dingoeMoving_toggle ? dingoeMoving2 : dingoeMoving1;
                            }
                            moveDingoe(currentDingoe, seeDirection, maze_bw);
                            printf("x = %d and y = %d\n", currentWombat->x, currentWombat->y);

                        }

                        // Draw dingoe
                        if (drawDingoe(currentDingoe) != 0) {
                            printf("Error: Failed to draw dingoe after move.\n");
                            return 1;
                        }

                        // Check collision
                        if (check_collision(currentDingoe, currentWombat)) {
                            printf("💥 Game Over! Wombat got caught!\n");
                            state = GAME_OVER;
                        }

                        // Check finished level
                        if (currentWombat->x > 675 && currentWombat->y > 425) {
                            printf("🏆 Game Won: Larry reached the destination!\n");
                            state = END;  
                        }
                    }
                    if (drawCursor(cursor)!=0){
                        printf("Error: Failed to draw cursor.\n");
                        return 1;
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


