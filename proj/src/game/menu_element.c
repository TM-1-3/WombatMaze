#include "menu_element.h"

MenuElement* loadMenuElement(uint16_t x, uint16_t y, xpm_map_t xpm) {
    // Allocate memory for the element
    MenuElement* menuElement = (MenuElement*) malloc(sizeof(MenuElement));
    if (menuElement == NULL) {
        printf("Error: Memory allocation for menu element failed.\n");
        return NULL;  
    }

    // Initialize element's position and load its sprite
    menuElement->x = x;
    menuElement->y = y;
    menuElement->menuElementSprite = loadSprite(xpm);
    if (menuElement->menuElementSprite == NULL) {
        printf("Error: Failed to load menu element sprite.\n");
        free(menuElement);
        return NULL;
    }
    return menuElement;
}

// Gets the x-coordinate of the element
int getMenuElementX(MenuElement* menuElement) {
    return menuElement ? menuElement->x : 0;  
}

// Sets the x-coordinate of the element
void setMenuElementX(MenuElement* menuElement, uint16_t x) {
    if (menuElement) {
        menuElement->x = x;
    }
}

// Gets the y-coordinate of the element
int getMenuElementY(MenuElement* menuElement) {
    return menuElement ? menuElement->y : 0; 
}

// Sets the y-coordinate of the element
void setMenuElementY(MenuElement* menuElement, uint16_t y) {
    if (menuElement) {
        menuElement->y = y;
    }
}

int drawMenuElement(MenuElement* menuElement){
    if (menuElement == NULL || menuElement->menuElementSprite == NULL) {
        printf("Error: Invalid menu element or menu element sprite.\n");
        return 1; 
    }
    // Draw the element using its sprite at its current position
    return drawSprite(menuElement->menuElementSprite, menuElement->x, menuElement->y);
}

void destroyMenuElement(MenuElement* menuElement) {
    if (menuElement == NULL) return;
    if (menuElement->menuElementSprite) {
         destroySprite(menuElement->menuElementSprite);
    }
    free(menuElement);
}


