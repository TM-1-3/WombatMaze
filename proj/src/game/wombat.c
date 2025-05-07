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

int moveHandler(uint8_t scan_code) {
   bool is_break = (scan_code & MAKE_CODE);
   uint8_t base_code = scan_code & ~MAKE_CODE;

   switch (base_code) {
       case MAKE_W: return is_break ? 0 : 1;
       case MAKE_S: return is_break ? 0 : 2;
       case MAKE_A: return is_break ? 0 : 3;
       case MAKE_D: return is_break ? 0 : 4;
       default: return -1;
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


