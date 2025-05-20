#include "wombat.h"
extern uint8_t scanCode;

// Loads a wombat object with a sprite
Wombat* loadWombat(uint16_t x, uint16_t y, Sprite* sprite) {

    // Allocate memory for the wombat
    Wombat* wombat = (Wombat*) malloc(sizeof(Wombat));
    if (wombat == NULL) {
        printf("Error: Memory allocation for wombat failed.\n");
        return NULL;  
    }

    // Initialize wombat's position and load its sprite
    wombat->x = x;
    wombat->y = y;
    wombat->wombatSprite = sprite;
    if (wombat->wombatSprite == NULL) {
        printf("Error: Failed to load wombat sprite.\n");
        free(wombat);
        return NULL;
    }
    return wombat;
}

// Gets the x-coordinate of the wombat
uint16_t getWombatX(Wombat* wombat) {
    return wombat ? wombat->x : 0;  
}

// Sets the x-coordinate of the wombat
void setWombatX(Wombat* wombat, uint16_t x) {
    if (wombat) {
        wombat->x = x;
    }
}

// Gets the y-coordinate of the wombat
uint16_t getWombatY(Wombat* wombat) {
    return wombat ? wombat->y : 0; 
}

// Sets the y-coordinate of the wombat
void setWombatY(Wombat* wombat, uint16_t y) {
    if (wombat) {
        wombat->y = y;
    }
}

// Draws the wombat on the screen
int drawWombat(Wombat* wombat) {
    if (wombat == NULL || wombat->wombatSprite == NULL) {
        printf("Error: Invalid wombat or wombat sprite.\n");
        return 1; 
    }

    // Draw the wombat using its sprite at its current position
    return drawSprite(wombat->wombatSprite, wombat->x, wombat->y);
}

// Handles movement input for the wombat
int moveHandler(uint8_t scan_code) {
    bool isBreak = (scan_code & MAKE_CODE);  
    uint8_t baseCode = scan_code & ~MAKE_CODE;  

    // Handle movement based on the pressed key
    switch (baseCode) {
        case MAKE_W: return isBreak ? 0 : 1; 
        case MAKE_S: return isBreak ? 0 : 2;  
        case MAKE_A: return isBreak ? 0 : 3;  
        case MAKE_D: return isBreak ? 0 : 4;  
        default: return -1;
    }
}

// Moves the wombat in a specified direction
void moveWombat(Wombat* wombat, int moveDirection, Maze* maze, Obstacle* obstacles[], int num_obstacles) {
    if (wombat == NULL) {
        printf("Error: Wombat is NULL.\n");
        return;
    }

    uint16_t x1, y1, x2, y2;

    switch (moveDirection) {
        case 1:
            x1 = wombat->x + 25;
            y1 = wombat->y + 25 - 3;
            x2 = wombat->x + 100 - 25;
            y2 = wombat->y + 25 - 3;
            if (wombat->y > 0 &&
                isPath(maze, x1, y1) && isPath(maze, x2, y2) &&
                !isAnyObstacle(obstacles, num_obstacles, x1, y1) &&
                !isAnyObstacle(obstacles, num_obstacles, x2, y2)) {
                wombat->y -= 3;
            }
            break;

        case 2:
            x1 = wombat->x + 25;
            y1 = wombat->y + 100 - 25 + 3;
            x2 = wombat->x + 100 - 25;
            y2 = wombat->y + 100 - 25 + 3;
            if (wombat->y + 100 < SCREEN_HEIGHT &&
                isPath(maze, x1, y1) && isPath(maze, x2, y2) &&
                !isAnyObstacle(obstacles, num_obstacles, x1, y1) &&
                !isAnyObstacle(obstacles, num_obstacles, x2, y2)) {
                wombat->y += 3;
            }
            break;

        case 3: 
            x1 = wombat->x + 25 - 3;
            y1 = wombat->y + 25;
            x2 = wombat->x + 25 - 3;
            y2 = wombat->y + 100 - 25;
            if (wombat->x > 0 &&
                isPath(maze, x1, y1) && isPath(maze, x2, y2) &&
                !isAnyObstacle(obstacles, num_obstacles, x1, y1) &&
                !isAnyObstacle(obstacles, num_obstacles, x2, y2)) {
                wombat->x -= 3;
            }
            break;

        case 4: 
            x1 = wombat->x + 100 - 25 + 3;
            y1 = wombat->y + 25;
            x2 = wombat->x + 100 - 25 + 3;
            y2 = wombat->y + 100 - 25;
            if (wombat->x + 100 < SCREEN_WIDTH &&
                isPath(maze, x1, y1) && isPath(maze, x2, y2) &&
                !isAnyObstacle(obstacles, num_obstacles, x1, y1) &&
                !isAnyObstacle(obstacles, num_obstacles, x2, y2)) {
                wombat->x += 3;
            }
            break;

        default:
            printf("Error: Invalid move direction.\n");
    }
}
