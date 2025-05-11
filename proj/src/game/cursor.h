#ifndef CURSOR_H
#define CURSOR_H
#include <lcom/lcf.h>
#include "../io/video/video.h"
#include "../display/sprite.h"

typedef struct {
    int x;
    int y;
    Sprite *cursorSprite;
} Cursor;

Cursor* loadCursor(uint16_t x, uint16_t y, xpm_map_t xpm);

int getCursorX(Cursor* cursor);

void setCursorX(Cursor* cursor, uint16_t x);

int getCursorY(Cursor* cursor);

void setCursorY(Cursor* cursor, uint16_t y);

int drawCursor(Cursor* cursor);

#endif


