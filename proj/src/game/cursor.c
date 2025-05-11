#include "cursor.h"

Cursor* loadCursor(uint16_t x, uint16_t y, xpm_map_t xpm) {

    // Allocate memory for the cursor
    Cursor* cursor = (Cursor*) malloc(sizeof(Cursor));
    if (cursor == NULL) {
        printf("Error: Memory allocation for cursor failed.\n");
        return NULL;  
    }

    // Initialize cursor's position and load its sprite
    cursor->x = x;
    cursor->y = y;
    cursor->cursorSprite = loadSprite(xpm);
    if (cursor->cursorSprite == NULL) {
        printf("Error: Failed to load cursor sprite.\n");
        free(cursor);
        return NULL;
    }
    return cursor;
}

int getCursorX(Cursor* cursor) {
    return cursor ? cursor->x : 0;  
}

// Sets the x-coordinate of the cursor
void setCursorX(Cursor* cursor, uint16_t x) {
    if (cursor) {
        cursor->x = x;
    }
}

// Gets the y-coordinate of the cursor
int getCursorY(Cursor* cursor) {
    return cursor ? cursor->y : 0; 
}

// Sets the y-coordinate of the cursor
void setCursorY(Cursor* cursor, uint16_t y) {
    if (cursor) {
        cursor->y = y;
    }
}

int drawCursor(Cursor* cursor){
    if (cursor == NULL || cursor->cursorSprite == NULL) {
        printf("Error: Invalid cursor or cursor sprite.\n");
        return 1; 
    }
    // Draw the cursor using its sprite at its current position
    return drawSprite(cursor->cursorSprite, cursor->x, cursor->y);
}


