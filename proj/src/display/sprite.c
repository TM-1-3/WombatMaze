#include "sprite.h"
#include <stdlib.h>  // for malloc
#include <stdint.h>  // for uint32_t
#include <stdio.h>   // for printf
#include "../../assets/numbers/zero.xpm"
#include "../../assets/numbers/one.xpm"
#include "../../assets/numbers/two.xpm"
#include "../../assets/numbers/three.xpm"
#include "../../assets/numbers/four.xpm"
#include "../../assets/numbers/five.xpm"
#include "../../assets/numbers/six.xpm"
#include "../../assets/numbers/seven.xpm"
#include "../../assets/numbers/eight.xpm"
#include "../../assets/numbers/nine.xpm"

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

// Get pixel color
uint32_t getPixelColor(Sprite* sprite, uint16_t x, uint16_t y) {

    // Check if null
    if (sprite == NULL || sprite->colors == NULL) {
        printf("Error: Invalid sprite or sprite colors.\n");
        return 0;
    }

    // Check dimensions
    if (x >= sprite->width || y >= sprite->height) {
        printf("Error: Coordinates out of bounds (x: %d, y: %d)\n", x, y);
        return 0; 
    }
    return sprite->colors[y * sprite->width + x];
}

// Loads digits sprites from an XPM map
Sprite **loadDigitSprites() {
    Sprite **digits = malloc(10 * sizeof(Sprite*));
    if (!digits) return NULL;

    xpm_map_t xpms[] = {zero_xpm, one_xpm, two_xpm, three_xpm, four_xpm,
                       five_xpm, six_xpm, seven_xpm, eight_xpm, nine_xpm};

    for (int i = 0; i < 10; i++) {
        digits[i] = loadSprite(xpms[i]);
        if (!digits[i]) {
            printf("Failed to load digit %d sprite\n", i);

            for (int j = 0; j < i; j++) {
                free(digits[j]->colors);
                free(digits[j]);
            }
            free(digits);
            return NULL;
        }
    }
    return digits;
}

//draw the numbers for the timer
int drawNumber(Sprite **digits, int number, int x, int y) {
    if (number < 0) return 1;
    
    char buffer[10];
    snprintf(buffer, sizeof(buffer), "%d", number);

    int num_digits = strlen(buffer);
    int digit_width = digits[0]->width;
    int spacing = 2;

    int total_width = num_digits * digit_width + (num_digits - 1) * spacing;
    int x_start = x - total_width;
    
    for (int i = 0; buffer[i] != '\0'; i++) {
        int digit = buffer[i] - '0';
        if (digit < 0 || digit > 9) continue;
        
        int x_pos = x_start + i * (digit_width + spacing);
        if (drawSprite(digits[digit], x_pos, y) != 0) {
            return 1;
        }
    }
    return 0;
}
