#include "game.h"

void reset_creatures(Wombat *wombat, Dingoe *dingoe, int level){
    if (level == 1) {
        setWombatX(wombat,  50);
        setWombatY(wombat,  50);
        setDingoeX(dingoe, 350);
        setDingoeY(dingoe, 50);
    }
    else if (level == 2) {
        setWombatX(wombat,  150);
        setWombatY(wombat,  150);
        setDingoeX(dingoe, 25);
        setDingoeY(dingoe, 475);
    }
    else if (level == 3) {
        setWombatX(wombat,  625);
        setWombatY(wombat,  475);
        setDingoeX(dingoe, 425);
        setDingoeY(dingoe, 225);
    }
}

void reset_obstacles(Obstacle* obstacles[], int level) {
    if (level == 1) {
        for (int i = 0; i < 2; i++) {
            obstacles[i]->active = true;
            obstacles[i]->x = 120 + (1 - i) * 50;
            obstacles[i]->y = 150;
        }
        for (int i = 2; i < 4; i++) {
            obstacles[i]->active = true;
            obstacles[i]->x = 130 + (3 - i) * 50;
            obstacles[i]->y = 350;
        }
    }
    else if (level == 2) {
        for (int i = 0; i < 2; i++) {
            obstacles[i]->active = true;
            obstacles[i]->x = 25 + (1 - i) * 50;
            obstacles[i]->y = 325;
        }
        for (int i = 2; i < 4; i++) {
            obstacles[i]->active = true;
            obstacles[i]->x = 300;
            obstacles[i]->y = 475 + (3 - i) * 50;;
        }
        for (int i = 4; i < 6; i++) {
            obstacles[i]->active = true;
            obstacles[i]->x = 575 + (5 - i) * 50;
            obstacles[i]->y = 325;
        }
        // for (int i = 6; i < 8; i++) {
        //     obstacles[i]->active = true;
        //     obstacles[i]->x = 50 + (7 - i) * 50;
        //     obstacles[i]->y = 350;
        // }
    }
    else if (level == 3) {
        for (int i = 0; i < 2; i++) {
            obstacles[i]->active = true;
            obstacles[i]->x = 175 + (1 - i) * 50;
            obstacles[i]->y = 100;
        }
        for (int i = 2; i < 4; i++) {
            obstacles[i]->active = true;
            obstacles[i]->x = 50 + (3 - i) * 50;
            obstacles[i]->y = 150;
        }
        // for (int i = 4; i < 6; i++) {
        //     obstacles[i]->active = true;
        //     obstacles[i]->x = 50 + (5 - i) * 50;
        //     obstacles[i]->y = 250;
        // }
        // for (int i = 6; i < 8; i++) {
        //     obstacles[i]->active = true;
        //     obstacles[i]->x = 50 + (7 - i) * 50;
        //     obstacles[i]->y = 350;
        // }
        // for (int i = 8; i < 10; i++) {
        //     obstacles[i]->active = true;
        //     obstacles[i]->x = 50 + (10 - i) * 50;
        //     obstacles[i]->y = 450;
        // }
        // for (int i = 10; i < 12; i++) {
        //     obstacles[i]->active = true;
        //     obstacles[i]->x = 50 + (11 - i) * 50;
        // //     obstacles[i]->y = 550;
        // }
    }
}




