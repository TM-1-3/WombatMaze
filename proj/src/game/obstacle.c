#include "obstacle.h"
#include "/display/sprite.h"
#include <stdlib.h>

//Create new obstacles
Obstacle* create_obstacle(uint16_t x, uint16_t y, xpm_map_t xpm) {
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
        return 1;
    }
    return drawSprite(obstacle->obstacleSprite, obstacle->x, obstacle->y);
}