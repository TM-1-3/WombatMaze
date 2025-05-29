#include "game.h"

void reset_creatures(Wombat *wombat, Dingoe *dingoe){
    setWombatX(wombat,  50);
    setWombatY(wombat,  50);
    setDingoeX(dingoe, 500);
    setDingoeY(dingoe, 50);
}

void reset_obstacles(Obstacle* obstacles[], int count) {
    for (int i = 0; i < count; i++) {
        obstacles[i]->active = true;
        obstacles[i]->x = 125 + i * 50;
        obstacles[i]->y = 400;
    }
}



