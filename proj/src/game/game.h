#ifndef GAME_H
#define GAME_H
#include <lcom/lcf.h>
#include "wombat.h"
#include "dingoe.h"

void reset_creatures(Wombat *wombat, Dingoe *dingoe);

void reset_obstacles(Obstacle* obstacles[], int count);

#endif


