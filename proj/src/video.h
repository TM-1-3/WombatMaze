#ifndef __VIDEO_H
#define __VIDEO_H

#include <lcom/lcf.h>
#include "VBE.h"
#include <math.h>

int (set_mode_graphic)(uint16_t mode);

int (build_frame_buffer)(uint16_t mode);

int (normalize_color)(uint32_t color, uint32_t *newColor);

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (draw_horizontal_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

#endif
