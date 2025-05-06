#include "wombat.h"

extern uint8_t scanCode;

Wombat* loadWombat(uint16_t x, uint16_t y, xpm_map_t xpm) {

    Wombat* wombat = (Wombat*) malloc(sizeof(Wombat));
    wombat->x = x;
    wombat->y = y;
    wombat->wombatSprite = loadSprite(xpm);
    return wombat;
}

uint16_t getX(Wombat* wombat){
    return wombat->x;
}

void setX(Wombat* wombat, uint16_t x){
    wombat->x=x;
}

uint16_t getY(Wombat* wombat){
    return wombat->y;
}

void setY(Wombat* wombat, uint16_t y){
    wombat->y=y;
}

int drawWombat(Wombat* wombat){
    if (drawSprite(wombat->wombatSprite, wombat->x, wombat->y)!=0){
        return 1;
    }
    return 0;
}

int moveHandler(uint8_t scanCode){
    switch (scanCode){
        case UP_MAKE_CODE:
           return 1;
        case UP_BREAK_CODE:
           return 0;
        case DOWN_MAKE_CODE:
           return 2;
        case DOWN_BREAK_CODE:
           return 0;
        case LEFT_MAKE_CODE:
           return 3;
        case LEFT_BREAK_CODE:
           return 0;
        case RIGHT_MAKE_CODE:
           return 4;
        case RIGHT_BREAK_CODE:
           return 0;
        default:
           return -1;
    }
}

void moveWombat(Wombat* wombat, int moveDirection){
    switch (moveDirection){
        case 1:
           setY(wombat, wombat->y-1);
           break;
        case 2:
           setY(wombat, wombat->y+1);
           break;
        case 3:
           setX(wombat, wombat->x-1);
           break;
        case 4:
           setX(wombat, wombat->x+1);
           break;
    }
}


