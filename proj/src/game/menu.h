#ifndef MENU_H
#define MENU_H
#include <lcom/lcf.h>
#include "menu_element.h"

/**
 * @brief Draws the full menu by rendering the logo, background, and cursor
 * @param logo Pointer to the logo MenuElement
 * @param menuBackground Pointer to the background MenuElement
 * @param menuCursor Pointer to the cursor MenuElement
 * @return 0 on success, non-zero on failure
 */
int drawMenu(MenuElement* logo, MenuElement* menuBackground, MenuElement* menuCursor);

/**
 * @brief Moves the menu cursor down by one element
 * @param menuCursor Pointer to the cursor MenuElement
 */
void menuCursorDown(MenuElement* menuCursor);

/**
 * @brief Moves the menu cursor up by one element
 * @param menuCursor Pointer to the cursor MenuElement
 */
void menuCursorUp(MenuElement* menuCursor);

#endif

