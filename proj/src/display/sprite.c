#include "sprite.h"

Sprite *loadSprite(xpm_map_t xpm){

    Sprite *sprite=(Sprite*)malloc(sizeof(Sprite));
    xpm_image_t img;
    sprite->colors=(uint32_t*)xpm_load(xpm, XPM_8_8_8_8, &img);
    sprite->height=img.height;
    sprite->width=img.width;
    return sprite;
}

int drawSprite(Sprite *sprite, int x, int y){
    for (int i=0; i<sprite->height; i++){
        for (int j=0; j<sprite->width; j++){
            if (draw_pixel(x+j, y+i, sprite->colors[j+i*sprite->width])!=0){
                return 1;
            }
        }
    }
    return 0;
}

