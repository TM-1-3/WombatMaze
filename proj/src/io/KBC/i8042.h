#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>    /* BIT macro */

/* KBC IRQ */
#define IRQ_KEYBOARD    1     /* Keyboard controller IRQ line */
#define IRQ_MOUSE       12    /* Mouse (auxiliary) IRQ line */

/* KBC Ports */
#define KBC_SR  0x64    /* Status register (read) */
#define KBC_CR     0x64    /* Command register (write) */
#define KBC_OB     0x60    /* Output buffer (read) */
#define KBC_IB      0x60    /* Input buffer (write) */

/* KBC Timing */
#define WAIT_KBC        20000   /* Max loops waiting for I/O */
#define MAX_ATTEMPTS    10      /* Max retries on NACK/errors */

/* KBC Commands */
#define KBC_READ    0x20    /* Read command byte */
#define KBC_WRITE   0x60    /* Write command byte */

/* KBC Flags */
#define ENABLE_KB   BIT(0)  /* Enable keyboard interrupts */
#define ENABLE_MOUSE BIT(1) /* Enable mouse interrupts */
#define DISABLE_KB       BIT(4) /* Disable keyboard */
#define DISABLE_MOUSE    BIT(5) /* Disable mouse */

/* Keyboard Masks */
#define STAT_OBF        BIT(0)  /* Output buffer full */
#define STAT_IBF        BIT(1)  /* Input buffer full */
#define STAT_SYS        BIT(2)  /* System flag */
#define STAT_A2         BIT(3)  /* Command/data */
#define STAT_INH        BIT(4)  /* Keyboard inhibit */
#define STAT_AUX        BIT(5)  /* Mouse output buffer */
#define STAT_TIMEOUT    BIT(6)  /* Timeout error */
#define STAT_PARITY     BIT(7)  /* Parity error */

/* Mouse Masks */
#define MOUSE_LB        BIT(0)  /* Left button */
#define MOUSE_RB        BIT(1)  /* Right button */
#define MOUSE_MB        BIT(2)  /* Middle button */
#define MOUSE_SYNC      BIT(3)  /* Always 1 */
#define MOUSE_XSIGNAL  BIT(4)  /* X sign bit */
#define MOUSE_YSIGNAL  BIT(5)  /* Y sign bit */
#define MOUSE_XOVER    BIT(6)  /* X overflow */
#define MOUSE_YOVER    BIT(7)  /* Y overflow */

/* Mouse Commands */
#define MOUSE_WRITE_CMD 0xD4    /* Write next byte to mouse */
#define ENABLE_STREAM_MODE 0xEA  /* Enables stream mode */
#define ENABLE_DATA_REPORT 0xF4  /* Enables data report */
#define DISABLE_DATA_REPORT 0xF5  /* Disables daa report */


/* Mouse Responses */
#define MOUSE_ACK       0xFA  /* Acknowledge */
#define MOUSE_NACK      0xFE  /* Resend request */
#define MOUSE_ERROR     0xFC  /* Error response */

/* Scancode prefixes */
#define MAKE_CODE       BIT(7)  /* Break code prefix */
#define TWO_BYTES       0xE0    /* Two-byte scan code */

/* Common Scancodes */
#define MAKE_ESC        0x01
#define BREAK_ESC       0x81
#define MAKE_ENTER      0x1C
#define BREAK_ENTER     0x9C
#define MAKE_BACKSPACE  0x0E
#define MAKE_MINUS      0x0C
#define MAKE_PLUS       0x0D

/* Number Scancodes */
#define MAKE_1          0x02
#define MAKE_2          0x03
#define MAKE_3          0x04
#define MAKE_4          0x05
#define MAKE_5          0x06
#define MAKE_6          0x07
#define MAKE_7          0x08
#define MAKE_8          0x09
#define MAKE_9          0x0A
#define MAKE_0          0x0B

/* Letter Scancodes */
#define MAKE_A          0x1E
#define MAKE_B          0x30
#define MAKE_C          0x2E
#define MAKE_D          0x20
#define MAKE_E          0x12
#define MAKE_F          0x21
#define MAKE_G          0x22
#define MAKE_H          0x23
#define MAKE_I          0x17
#define MAKE_J          0x24
#define MAKE_K          0x25
#define MAKE_L          0x26
#define MAKE_M          0x32
#define MAKE_N          0x31
#define MAKE_O          0x18
#define MAKE_P          0x19
#define MAKE_Q          0x10
#define MAKE_R          0x13
#define MAKE_S          0x1F
#define MAKE_T          0x14
#define MAKE_U          0x16
#define MAKE_V          0x2F
#define MAKE_W          0x11
#define MAKE_X          0x2D
#define MAKE_Y          0x15
#define MAKE_Z          0x2C

#define MAKE_LEFT       0x4B
#define BREAK_LEFT      0xCB

#define MAKE_RIGHT      0x4D
#define BREAK_RIGHT     0xCD

#define MAKE_UP         0x48
#define BREAK_UP        0xC8

#define MAKE_DOWN       0x50
#define BREAK_DOWN      0xD0

#endif /* _LCOM_I8042_H_ */
