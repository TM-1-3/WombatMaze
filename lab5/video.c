#include "video.h"

vbe_mode_info_t modeInfo;
uint8_t* frameBuffer;

int (set_mode_graphic)(uint16_t mode){
    
    reg86_t reg86;

    memset(&reg86, 0, sizeof(reg86));
    reg86.intno=0x10;
    reg86.ax = 0x4F02;
    reg86.bx = BIT(14) | mode;
    if (sys_int86(&reg86)!=0){
        return 1;
    }
    return 0;
}

int (build_frame_buffer)(uint16_t mode){
    memset(&modeInfo,0,sizeof(modeInfo));
    if (vbe_get_mode_info(mode, &modeInfo)){
        return 1;
    }

    unsigned int bytesPerPixel = (7+modeInfo.BitsPerPixel)/8;
    unsigned int frameSize = bytesPerPixel * modeInfo.XResolution * modeInfo.YResolution;

    struct minix_mem_range mr;
    mr.mr_base = modeInfo.PhysBasePtr;
    mr.mr_limit = frameSize+mr.mr_base;

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)){
        return 1;
    }
    frameBuffer = vm_map_phys(SELF, (void*)mr.mr_base, frameSize);
    if (frameBuffer==NULL){
        return 1;
    }
    //memset(frameBuffer,0xFF,frameSize);
    return 0;
}

int (normalize_color)(uint32_t color, uint32_t *newColor){
    if (modeInfo.BitsPerPixel==32){
        *newColor=0;
    }
    else{
        *newColor=color & (BIT(modeInfo.BitsPerPixel)-1);
    }
    return 0;
}

int (draw_pixel)(uint16_t x, uint16_t y, uint32_t color){
    if (x>modeInfo.XResolution || y>modeInfo.YResolution){
        return 1;
    }
    unsigned BytesPerPixel=(modeInfo.BitsPerPixel+7)/8;
    unsigned int pixelIndex=BytesPerPixel*(modeInfo.XResolution*y+x);
    if (memcpy(&frameBuffer[pixelIndex],&color,BytesPerPixel)==NULL){
        return 1;
    }
    return 0;
}

int (draw_horizontal_line)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for (unsigned i=0;i<len;i++){
        if (draw_pixel(x+i,y,color)!=0){
            return 1;
        }
    }
    return 0;
}

int (draw_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
    xpm_image_t img;
    uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
    for (int i=0; i<img.height; i++){
        for (int j=0; j<img.width; j++){
            if (draw_pixel(x+j, y+i, *colors)!=0){
                return 1;
            }
            colors++;
        }
    }
    return 0;
}
