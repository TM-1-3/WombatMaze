#include "obstacle.h"
#include <stdlib.h>

//Create new obstacles
Obstacle* load_obstacle(uint16_t x, uint16_t y, xpm_map_t xpm) {
    Obstacle* obstacle = (Obstacle*)malloc(sizeof(Obstacle));
    if (obstacle == NULL) {
        printf("Error: Failed to allocate memory for obstacle\n");
        return NULL;
    }
    
    obstacle->x = x;
    obstacle->y = y;
    obstacle->active = 1;
    obstacle->obstacleSprite = loadSprite(xpm);
    
    if (obstacle->obstacleSprite == NULL) {
        printf("Error: Failed to load obstacle sprite\n");
        free(obstacle);
        return NULL;
    }
    
    return obstacle;
}

//Marks an obstacle as inactive
void remove_obstacle(Obstacle* obstacle) {
    if (obstacle != NULL) {
        obstacle->active = 0;
    }
}

//Check if obstacle is active
uint8_t is_obstacle_active(Obstacle* obstacle) {
    return (obstacle != NULL) ? obstacle->active : 0;
}

//Draw the obstacle
int draw_obstacle(Obstacle* obstacle) {
    if (obstacle == NULL || !obstacle->active || obstacle->obstacleSprite == NULL) {
        printf("Obstacle active: %d\n", obstacle->active);
        return 1;
    }
    return drawSprite(obstacle->obstacleSprite, obstacle->x, obstacle->y);
}

//Check if mouse is over the obstacle
static bool is_mouse_over_obstacle(Obstacle* obstacle, uint16_t mouse_x, uint16_t mouse_y) {
    if (obstacle == NULL || obstacle->obstacleSprite == NULL) return false;

    uint16_t width = obstacle->obstacleSprite->width;
    uint16_t height = obstacle->obstacleSprite->height;

    return (mouse_x >= obstacle->x && mouse_x <= obstacle->x + width &&
            mouse_y >= obstacle->y && mouse_y <= obstacle->y + height);
}

//Updates the obstacle state based on the cursor position and mouse click
void update_obstacle_with_cursor(Obstacle* obstacle, Cursor* cursor, struct packet *pp) {
    if (obstacle == NULL || cursor == NULL || pp == NULL || !obstacle->active) return;

    uint16_t mouse_x = cursor->x;
    uint16_t mouse_y = cursor->y;

    if (pp->lb && is_mouse_over_obstacle(obstacle, mouse_x, mouse_y)) {
        remove_obstacle(obstacle);
    }
}

// Function to check if is obstacle
int isObstacle(Obstacle* obstacle, uint16_t x, uint16_t y) {
    if (obstacle == NULL || obstacle->obstacleSprite == NULL || !obstacle->active) {
        return 0;
    }

    uint16_t width = obstacle->obstacleSprite->width;
    uint16_t height = obstacle->obstacleSprite->height;


    if (x >= obstacle->x && x < obstacle->x + width &&
        y >= obstacle->y && y < obstacle->y + height) {

        uint32_t color = getPixelColor(obstacle->obstacleSprite, x, y);

        if (color != 0xFFFFFF) {
            return 1; 
        }
    }

    return 0;
}

int isAnyObstacle(Obstacle* obstacles[], int num_obstacles, uint16_t x, uint16_t y) {
    for (int i = 0; i < num_obstacles; i++) {
        if (isObstacle(obstacles[i], x, y)) {
            return 1;
        }
    }
    return 0;
}
