#include "dingoe.h"
#include "wombat.h"
extern uint8_t scanCode;

// Loads a dingoe object with a sprite
Dingoe* loadDingoe(uint16_t x, uint16_t y, xpm_map_t xpm) {

    // Allocate memory for the dingoe
    Dingoe* dingoe = (Dingoe*) malloc(sizeof(Dingoe));
    if (dingoe == NULL) {
        printf("Error: Memory allocation for dingoe failed.\n");
        return NULL;  
    }

    // Initialize dingoe's position and load its sprite
    dingoe->x = x;
    dingoe->y = y;
    dingoe->dingoeSprite = loadSprite(xpm);
    if (dingoe->dingoeSprite == NULL) {
        printf("Error: Failed to load dingoe sprite.\n");
        free(dingoe);
        return NULL;
    }
    return dingoe;
}

// Gets the x-coordinate of the dingoe
uint16_t getDingoeX(Dingoe* dingoe) {
    return dingoe ? dingoe->x : 0;  
}

// Sets the x-coordinate of the dingoe
void setDingoeX(Dingoe* dingoe, uint16_t x) {
    if (dingoe) {
        dingoe->x = x;
    }
}

// Gets the y-coordinate of the dingoe
uint16_t getDingoeY(Dingoe* dingoe) {
    return dingoe ? dingoe->y : 0; 
}

// Sets the y-coordinate of the dingoe
void setDingoeY(Dingoe* dingoe, uint16_t y) {
    if (dingoe) {
        dingoe->y = y;
    }
}

// Draws the dingoe on the screen
int drawDingoe(Dingoe* dingoe) {
    if (dingoe == NULL || dingoe->dingoeSprite == NULL) {
        printf("Error: Invalid dingoe or dingoe sprite.\n");
        return 1; 
    }

    // Draw the dingoe using its sprite at its current position
    return drawSprite(dingoe->dingoeSprite, dingoe->x, dingoe->y);
}

// Moves the dingoe in a specified direction
void moveDingoe(Dingoe* dingoe, int seeDirection, Maze* maze) {
    if (dingoe == NULL) {
        printf("Error: Dingoe is NULL.\n");
        return;
    }

    // Determine the new position based on the move direction
    switch (seeDirection) {
        case 1:
            if (dingoe->y > 0 &&
                isPath(maze, dingoe->x, dingoe->y - 1) &&
                isPath(maze, dingoe->x + 99, dingoe->y - 1)) {
                dingoe->y -= 1;
            }
            break;
        case 2:
            if (dingoe->y + 100 < SCREEN_HEIGHT &&
                isPath(maze, dingoe->x, dingoe->y + 100) &&
                isPath(maze, dingoe->x + 99, dingoe->y + 100)) {
                dingoe->y += 1;
            }
            break;
        case 3:
            if (dingoe->x > 0 &&
                isPath(maze, dingoe->x - 1, dingoe->y) &&
                isPath(maze, dingoe->x - 1, dingoe->y + 99)) {
                dingoe->x -= 1;
            }
            break;
        case 4:
            if (dingoe->x + 100 < SCREEN_WIDTH &&
                isPath(maze, dingoe->x + 100, dingoe->y) &&
                isPath(maze, dingoe->x + 100, dingoe->y + 99)) {
                dingoe->x += 1;
            }
            break;
        default:
            printf("Error: Invalid move direction.\n");
    }    
}

// Check if dingoe can see wombat 
int seeWombat(Dingoe* dingoe, Wombat* wombat, Maze* maze) {
    if (!dingoe || !wombat || !maze) return 0; 

    // Get the coordinates
    int tdx = getDingoeX(dingoe); 
    int tdy = getDingoeY(dingoe);
    int bdx = getDingoeX(dingoe) + 100; 
    int bdy = getDingoeY(dingoe) + 100;
    int twx = getWombatX(wombat);
    int twy = getWombatY(wombat);
    int bwx = getWombatX(wombat) + 100;
    int bwy = getWombatY(wombat) + 100;

    // Check all horizontally
    for (int i = tdy; i <= bdy; i++) {
        for (int j = twy; j <= bwy; j++) {
            if (i == j) {

                // Dingoe will move right
                if (tdx < bwx) {
                    for (int c = tdx + 100; c <= bwx - 100; c += 25) {
                        if (!isPath(maze, c, i)) return 0; 
                    }
                    return 4; 
                } 

                // Dingoe will move left
                if (bdx > twx) {
                    for (int c = bdx - 100; c >= twx + 100; c -= 25) {
                        if (!isPath(maze, c, i)) return 0; 
                    }
                    return 3; 
                }
            }
        }
    }

    // Check all vertically
    for (int i = tdx; i <= bdx; i++) {
        for (int j = twx; j <= bwx; j++) {
            if (i == j) {

                // Dingoe will move down
                if (tdy < bwy) {
                    for (int c = tdy + 100; c <= bwy - 100; c += 25) {
                        if (!isPath(maze, i, c)) return 0; 
                    }
                    return 2;  
                } 

                // Dingoe will move up
                if (bdy > twy) {
                    for (int c = bdy - 100; c >= twy + 100; c -= 25) {
                        if (!isPath(maze, i, c)) return 0; 
                    }
                    return 1;  
                }
            }
        }
    }
    return 0;  
}

// Check collision with wombat
bool check_collision(Dingoe* dingoe, Wombat* wombat) {
    if (!wombat || !dingoe) return false;

    int wx = getWombatX(wombat);
    int wy = getWombatY(wombat);
    int dx = getDingoeX(dingoe);
    int dy = getDingoeY(dingoe);

    int size = 100; // assuming both are 100x100

    return (wx < dx + size &&
            wx + size > dx &&
            wy < dy + size &&
            wy + size > dy);
}




