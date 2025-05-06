#ifndef WOMBAT_H
#define WOMBAT_H

#include <lcom/lcf.h>
#include "video.h"
#include "sprite.h"
#include "i8042.h"

typedef struct {
    uint16_t x;
    uint16_t y;
    Sprite *wombatSprite;
} Wombat;

Wombat* loadWombat(uint16_t x, uint16_t y, xpm_map_t xpm);

uint16_t getX(Wombat* wombat);

void setX(Wombat* wombat, uint16_t x);

uint16_t getY(Wombat* wombat);

void setY(Wombat* wombat, uint16_t y);

int drawWombat(Wombat* wombat);

int moveHandler(uint8_t scanCode);

void moveWombat(Wombat* wombat, int moveDirection);

#endif
