#ifndef __VIDEO_H
#define __VIDEO_H
#include <lcom/lcf.h>
#include "VBE.h"
#include "COLORS.h"
#include <math.h>

// Setup
extern uint8_t* frameBuffer;
extern uint8_t* doubleBuffer;

/**
 * @brief Sets the graphics mode 
 * @param mode VBE mode to set
 * @return 0 on success, non-zero otherwise
 */
int (set_mode_graphic)(uint16_t mode);

/**
 * @brief Allocates and maps the frame buffer 
 * @param mode VBE graphics mode
 * @return 0 on success, non-zero otherwise
 */
int (build_frame_buffer)(uint16_t mode);

/**
 * @brief Normalizes a 32-bit RGB color 
 * @param color Original 32-bit RGB color
 * @param newColor Pointer to store the normalized color
 * @return 0 on success, non-zero otherwise
 */
int (normalize_color)(uint32_t color, uint32_t *newColor);

/**
 * @brief Draws a single pixel 
 * @param x Horizontal position
 * @param y Vertical position
 * @param color Color of the pixel 
 * @return 0 on success, non-zero otherwise
 */
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a filled rectangle on the screen.
 * @param x The x-coordinate of the top-left
 * @param y The y-coordinate of the top-left 
 * @param width The width of the rectangle
 * @param height The height of the rectangle
 * @param color The color of the rectangle
 * @return 0 on success, non-zero otherwise
 */
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color);

/**
 * @brief Draws a horizontal line
 * @param x Starting x coordinate
 * @param y Y coordinate
 * @param len Length of the line in pixels
 * @param color Color of the line
 * @return 0 on success, non-zero otherwise
 */
int (draw_horizontal_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws an XPM image
 * @param xpm XPM image to draw
 * @param x X coordinate of the top-left corner
 * @param y Y coordinate of the top-left corner
 * @return 0 on success, non-zero otherwise
 */
int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Swaps the two buffers
 * @return 0 on success, non-zero otherwise
 */
int (swap_buffers)();

#endif
