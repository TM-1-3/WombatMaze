#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

#include <lcom/lcf.h>    /* for BIT macro */
#include <stdint.h>

/* Screen size */
#define SCREEN_WIDTH  700    
#define SCREEN_HEIGHT 500

/* VBE indices */
#define INDEXED_768       0x105  /* 1024x768 indexed color */
#define DIRECT_480        0x110  /* 640x480 direct color */
#define DIRECT_600        0x115  /* 800x600 direct color */
#define DIRECT_1024       0x11A  /* 1024x768 direct color */
#define DIRECT_864        0x14C  /* 1280x1024 direct color */

/* VBE function */
#define CTRL_INFO         0x4F00  /* Return VBE controller info */
#define MODE_INFO         0x4F01  /* Return VBE mode info */
#define SET_MODE          0x4F02  /* Set VBE mode */

/* VBE attributes */
#define LINEAR_FRAMEBUFFER     BIT(14) /* Linear framebuffer bit */

/* Color modes */
#define MODEL_DIRECT     0x06    /* Direct color model */
#define MODEL_INDEXED    0x00    /* Indexed color model */

#endif /* _LCOM_VBE_H_ */
