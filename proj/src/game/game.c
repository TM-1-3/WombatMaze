#include "game.h"

void reset_creatures(Wombat *wombat, Dingoe *dingoe, int level){
    if (level == 1) {
        setWombatX(wombat,  50);
        setWombatY(wombat,  50);
        setDingoeX(dingoe, 350);
        setDingoeY(dingoe, 50);
    }
    else if (level == 2) {
        setWombatX(wombat,  625);
        setWombatY(wombat,  475);
        setDingoeX(dingoe, 300);
        setDingoeY(dingoe, 225);
    }
    else if (level == 3) {
        setWombatX(wombat,  150);
        setWombatY(wombat,  150);
        setDingoeX(dingoe, 25);
        setDingoeY(dingoe, 475);
    }
}

void reset_obstacles(Obstacle* obstacles[], int count, int level) {
    if (level == 1) {
        for (int i = 0; i < count; i++) {
            obstacles[i]->active = true;
            obstacles[i]->x = 100 + i * 50;
            obstacles[i]->y = 100;
        }
    }
    else if (level == 2) {
        for (int i = 0; i < count; i++) {
            obstacles[i]->active = true;
            obstacles[i]->x = 125 + i * 50;
            obstacles[i]->y = 400;
        }
    }
    else if (level == 3) {
        for (int i = 0; i < count; i++) {
            obstacles[i]->active = true;
            obstacles[i]->x = 125 + i * 50;
            obstacles[i]->y = 400;
        }
    }
}




