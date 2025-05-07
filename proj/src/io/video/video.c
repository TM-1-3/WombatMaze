#include "video.h"

// Setup
vbe_mode_info_t modeInfo;
uint8_t* frameBuffer;
uint8_t* doubleBuffer;

// Sets the video mode to graphics mode
int (set_mode_graphic)(uint16_t mode){
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));
    reg86.intno = 0x10;
    reg86.ax = SET_MODE;
    reg86.bx = LINEAR_FRAMEBUFFER | mode;

    // Call the interrupt
    if (sys_int86(&reg86) != 0){
        printf("Error: Failed to set video mode 0x%X.\n", mode);
        return 1;
    }
    return 0;
}

// Builds the frame buffer
int (build_frame_buffer)(uint16_t mode){
    memset(&modeInfo, 0, sizeof(modeInfo));

    // Get information about the mode
    if (vbe_get_mode_info(mode, &modeInfo)){
        printf("Error: Failed to get mode info for mode 0x%X.\n", mode);
        return 1;
    }

    // Calculates the data
    unsigned int bytesPerPixel = (7 + modeInfo.BitsPerPixel) / 8;
    unsigned int frameSize = bytesPerPixel * modeInfo.XResolution * modeInfo.YResolution;

    // Set the data
    struct minix_mem_range mr;
    mr.mr_base = modeInfo.PhysBasePtr;
    mr.mr_limit = frameSize + mr.mr_base;

    // Add memory mapping
    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)){
        printf("Error: Failed to add memory mapping.\n");
        return 1;
    }

    // Map the memory
    frameBuffer = vm_map_phys(SELF, (void*)mr.mr_base, frameSize);
    if (frameBuffer == NULL){
        printf("Error: Failed to map memory for framebuffer.\n");
        return 1;
    }

    // Double buffering
    doubleBuffer = malloc(frameSize);
    if (doubleBuffer == NULL) {
        printf("Error: Failed to allocate double buffer.\n");
        return 1;
    }

    //memset(frameBuffer, 0xFF, frameSize); // Optional clearing of the framebuffer
    return 0;
}

// Normalizes a color
int (normalize_color)(uint32_t color, uint32_t *newColor){
    
    // Check if 32-bit
    if (modeInfo.BitsPerPixel == 32){
        *newColor = 0;
    }

    // Mask the color 
    else{
        *newColor = color & (BIT(modeInfo.BitsPerPixel) - 1);
    }
    return 0;
}

// Draws a single pixel
int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color){

    // Check if out of bounds
    if (x > modeInfo.XResolution || y > modeInfo.YResolution){
        printf("Error: Coordinates (x: %d, y: %d) out of bounds.\n", x, y);
        return 1;
    }

    // Calculate the pixel's index
    unsigned bpp = (modeInfo.BitsPerPixel + 7) / 8;
    unsigned int pixelIndex = bpp * (modeInfo.XResolution * y + x);

    // Copy the color value
    if (memcpy(&doubleBuffer[pixelIndex], &color, bpp) == NULL){
        printf("Error: Failed to copy color value to doubleBuffer.\n");
        return 1;
    }
    return 0;
}

// Draws a filled rectangle on the screen
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color){
    for (unsigned i = 0; i < height; i++){
      if (draw_horizontal_line(x, y + i, width, color) != 0){
        vg_exit();
        return 1;
      }
    }
    return 0;
}

// Draws a horizontal line
int (draw_horizontal_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){

    // Draws each pixel
    for (unsigned i = 0; i < len; i++){
        if (draw_pixel(x + i, y, color) != 0){
            printf("Error: Failed to draw pixel at (x: %d, y: %d).\n", x + i, y);
            return 1;
        }
    }
    return 0;
}

// Draws an XPM image
int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
    xpm_image_t img;

    // Load the XPM into memory
    uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
    if (colors == NULL) {
        printf("Error: Failed to load XPM image.\n");
        return 1;
    }

    // Draw each pixel
    for (int i = 0; i < img.height; i++){
        for (int j = 0; j < img.width; j++){
            if (draw_pixel(x + j, y + i, *colors) != 0){
                printf("Error: Failed to draw pixel at (x: %d, y: %d) while rendering XPM.\n", x + j, y + i);
                return 1;
            }
            colors++;
        }
    }
    return 0;
}

// Swaps the two buffers
int (swap_buffers)(){
    uint32_t size = modeInfo.XResolution * modeInfo.YResolution * (modeInfo.BitsPerPixel / 8);
    if (memcpy(frameBuffer, doubleBuffer, size) == NULL) {
        printf("Error: Failed to swap buffers.\n");
        return 1;
    }
    return 0;
}

// Clear the screen
void (clear_screen)(){
    memset(doubleBuffer, 0, modeInfo.XResolution * modeInfo.YResolution * (modeInfo.BitsPerPixel / 8));
}

