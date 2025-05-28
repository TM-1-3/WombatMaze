#ifndef MENU_H
#define MENU_H
#include <lcom/lcf.h>
#include "menu_element.h"

int drawMenu(MenuElement* logo, MenuElement* menuBackground, MenuElement* menuCursor);

void menuCursorDown(MenuElement* menuCursor);

void menuCursorUp(MenuElement* menuCursor);

#endif

