#ifndef MENU_ELEMENT_H
#define MENU_ELEMENT_H
#include <lcom/lcf.h>
#include "../io/video/video.h"
#include "../display/sprite.h"

typedef struct {
    uint16_t x;
    uint16_t y;
    Sprite *menuElementSprite;
} MenuElement;

MenuElement* loadMenuElement(uint16_t x, uint16_t y, xpm_map_t xpm);

int getMenuElementX(MenuElement* menuElement);

void setMenuElementX(MenuElement* menuElement, uint16_t x);

int getMenuElementY(MenuElement* menuElement);

void setMenuElementY(MenuElement* menuElement, uint16_t y);

int drawMenuElement(MenuElement* menuElement);


#endif


