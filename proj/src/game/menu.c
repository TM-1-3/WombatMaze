#include "menu.h"

int drawMenu(MenuElement* logo, MenuElement* menuBackground, MenuElement* menuCursor){
    if (drawMenuElement(menuBackground)!=0){
        printf("Error: Failed to draw background sprite.\n");
        return 1;
    }
    if (drawMenuElement(logo)!=0){
        printf("Error: Failed to draw logo sprite.\n");
        return 1;
    }
    if (drawMenuElement(menuCursor)!=0){
        printf("Error: Failed to draw menu cursor sprite.\n");
        return 1;
    }
    return 0;
}

void menuCursorUp(MenuElement* menuCursor){
    if (getMenuElementY(menuCursor) == 255){
        menuCursor->x = 160;
        menuCursor->y = 255;
    }
    if (getMenuElementY(menuCursor) == 335){
        menuCursor->x = 160;
        menuCursor->y = 255;
    }
    if (getMenuElementY(menuCursor) == 405){
        menuCursor->x = 140;
        menuCursor->y = 335;
    }
}


void menuCursorDown(MenuElement* menuCursor){
    if (getMenuElementY(menuCursor) == 255){
        menuCursor->x = 140;
        menuCursor->y = 335;
    }
    else if (getMenuElementY(menuCursor) == 335){
        menuCursor->x = 260;
        menuCursor->y = 405;
    }
    else if (getMenuElementY(menuCursor) == 405){
        menuCursor->x = 260;
        menuCursor->y = 405;
    }
}


