#ifndef _SPRITE_H
#define _SPRITE_H
#include "../io/video/video.h"


typedef struct Sprite{
    uint16_t height;
    uint16_t width;
    uint32_t *colors;
} Sprite;

Sprite *loadSprite(xpm_map_t xpm);

int drawSprite(Sprite *sprite, int x, int y);

#endif

