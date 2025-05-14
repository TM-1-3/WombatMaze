#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <stdint.h>

// Obstacle structure definition
typedef struct {
    uint16_t x, y;          // Obstacle position coordinates
    uint8_t active;         // Flag indicating if obstacle is active (1) or removed (0)
    Sprite* obstacleSprite; // Pointer to obstacle's sprite
    // Add other attributes as needed (e.g., type, health points)
} Obstacle;

/**
 * @brief Creates a new obstacle instance
 * @param x X-coordinate position
 * @param y Y-coordinate position
 * @param xpm XPM map for the obstacle sprite
 * @return Pointer to created Obstacle, NULL on failure
 */
Obstacle* create_obstacle(uint16_t x, uint16_t y, xpm_map_t xpm);

/**
 * @brief Marks an obstacle for removal (sets as inactive)
 * @param obstacle Pointer to the obstacle to remove
 */
void remove_obstacle(Obstacle* obstacle);

/**
 * @brief Checks if an obstacle is active
 * @param obstacle Pointer to the obstacle to check
 * @return 1 if active, 0 if inactive or NULL
 */
uint8_t is_obstacle_active(Obstacle* obstacle);

#endif