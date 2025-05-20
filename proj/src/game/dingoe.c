#include "dingoe.h"
#include "wombat.h"
extern uint8_t scanCode;

// Loads a dingoe object with a sprite
Dingoe* loadDingoe(uint16_t x, uint16_t y, Sprite* sprite) {

    // Allocate memory for the dingoe
    Dingoe* dingoe = (Dingoe*) malloc(sizeof(Dingoe));
    if (dingoe == NULL) {
        printf("Error: Memory allocation for dingoe failed.\n");
        return NULL;  
    }

    // Initialize dingoe's position and load its sprite
    dingoe->x = x;
    dingoe->y = y;
    dingoe->dingoeSprite = sprite;
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

// Moves the wombat in a specified direction
void moveDingoe(Dingoe* dingoe, int seeDirection, Maze* maze) {
    if (dingoe == NULL) {
        printf("Error: Dingoe is NULL.\n");
        return;
    }

    // Determine the new position based on the see direction
    switch (seeDirection) {
        case 1:
            if (dingoe->y > 0 &&
                isPath(maze, dingoe->x + 25, dingoe->y + 25 - 1) &&
                isPath(maze, dingoe->x + 100 - 25, dingoe->y + 25 - 1)) {
                dingoe->y -= 1;
            }
            break;
        case 2:
            if (dingoe->y + 100 < SCREEN_HEIGHT &&
                isPath(maze, dingoe->x + 25, dingoe->y + 100 - 25 + 1) &&
                isPath(maze, dingoe->x + 100 - 25, dingoe->y + 100 - 25 + 1)) {
                dingoe->y += 1;
            }
            break;
        case 3:
            if (dingoe->x > 0 &&
                isPath(maze, dingoe->x + 25 - 1, dingoe->y + 25) &&
                isPath(maze, dingoe->x + 25 - 1, dingoe->y + 100 - 25)) {
                dingoe->x -= 1;
            }
            break;
        case 4:
            if (dingoe->x + 100 < SCREEN_WIDTH &&
                isPath(maze, dingoe->x + 100 - 25 + 1, dingoe->y + 25) &&
                isPath(maze, dingoe->x + 100 - 25 + 1, dingoe->y + 100 - 25)) {
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
    int tdx = getDingoeX(dingoe) + 25; 
    int tdy = getDingoeY(dingoe) + 25;
    int bdx = getDingoeX(dingoe) + 100 - 25; 
    int bdy = getDingoeY(dingoe) + 100 - 25;
    int twx = getWombatX(wombat) + 25;
    int twy = getWombatY(wombat) + 25;
    int bwx = getWombatX(wombat) + 100 - 25;
    int bwy = getWombatY(wombat) + 100 - 25;

    // Check all horizontally
    for (int i = tdy; i <= bdy; i++) {
        for (int j = twy; j <= bwy; j++) {
            if (i == j) {

                // Dingoe will move right
                if (tdx < bwx &&
                    isPath(maze, bdx + 50, tdy) &&
                    isPath(maze, bdx + 50, bdy)) {
                    for (int c = bdx; c <= twx; c += 25) {
                        if (!isPath(maze, c, i)) return 0; 
                    }
                    return 4; 
                } 

                // Dingoe will move left
                if (bdx > twx &&
                    isPath(maze, tdx - 50, tdy) &&
                    isPath(maze, tdx - 50, bdy)) {
                    for (int c = tdx; c >= bwx; c -= 25) {
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
                if (tdy < bwy &&
                    isPath(maze, tdx, bdy + 50) &&
                    isPath(maze, bdx, bdy + 50)) {
                    for (int c = bdy; c <= twy; c += 25) {
                        if (!isPath(maze, i, c)) return 0; 
                    }
                    return 2;  
                } 

                // Dingoe will move up
                if (bdy > twy &&
                isPath(maze, tdx, tdy - 50) &&
                isPath(maze, bdx, tdy - 50)) {
                    for (int c = tdy; c >= bwy; c -= 25) {
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

    // Get coordinates
    int wx = getWombatX(wombat) + 25;
    int wy = getWombatY(wombat) + 25;
    int dx = getDingoeX(dingoe) + 25;
    int dy = getDingoeY(dingoe) + 25; 

    // Check collision
    int size = 50; 
    return (wx < dx + size &&
            wx + size > dx &&
            wy < dy + size &&
            wy + size > dy);
}




