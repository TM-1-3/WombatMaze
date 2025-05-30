#ifndef GAME_H
#define GAME_H
#include <lcom/lcf.h>
#include "wombat.h"
#include "dingoe.h"

/**
 * @brief Resets the state of the creatures wombat and dingoe for the given level
 * @param wombat Pointer to the Wombat creature to reset
 * @param dingoe Pointer to the Dingoe creature to reset
 * @param level The current game level to reset creatures for
 */
void reset_creatures(Wombat *wombat, Dingoe *dingoe, int level);

/**
 * @brief Resets an array of obstacles for the given level
 * @param obstacles Array of pointers to Obstacle objects
 * @param level The current game level to reset obstacles for
 */
void reset_obstacles(Obstacle* obstacles[], int level);

#endif


