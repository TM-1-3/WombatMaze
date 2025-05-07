#include "sprite.h"
#include <stdlib.h>  // for malloc
#include <stdint.h>  // for uint32_t
#include <stdio.h>   // for printf

// Loads a sprite from an XPM map
Sprite *loadSprite(xpm_map_t xpm) {

    // Allocate memory for sprite
    Sprite *sprite = (Sprite *)malloc(sizeof(Sprite));
    if (sprite == NULL) {
        printf("Error: Memory allocation for sprite failed.\n");
        return NULL;
    }

    // Load the XPM image
    xpm_image_t img;
    sprite->colors = (uint32_t *)xpm_load(xpm, XPM_8_8_8_8, &img);
    if (sprite->colors == NULL) {
        printf("Error: Failed to load XPM image.\n");
        free(sprite); 
        return NULL;
    }

    // Set sprite dimensions
    sprite->height = img.height;
    sprite->width = img.width;
    return sprite;
}

// Draws the sprite on the screen
int drawSprite(Sprite *sprite, int x, int y) {

    // Ensure the sprite is valid
    if (sprite == NULL || sprite->colors == NULL) {
        printf("Error: Invalid sprite or sprite colors.\n");
        return 1;  
    }

    // Loop through the sprite's pixels and draw them
    for (int i = 0; i < sprite->height; i++) {
        for (int j = 0; j < sprite->width; j++) {
            uint32_t color = sprite->colors[j + i * sprite->width]; 

            // If drawing a pixel fails, return error and print a message
            if ((draw_pixel)(x + j, y + i, color) != 0) {
                printf("Error: Failed to draw pixel at (%d, %d)\n", x + j, y + i);
                return 1;
            }
        }
    }
    return 0;  
}
