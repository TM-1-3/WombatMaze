#include "menu.h"

int drawMenu(MenuElement* logo){
    if (drawMenuElement(logo)!=0){
        printf("Error: Failed to draw logo sprite.\n");
        return 1;
    }
    printf("Drew logo\n");
    return 0;
}


