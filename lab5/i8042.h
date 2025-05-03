#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#define DELAY_US    20000

#define IRQ_KEYBOARD 1

#define KBC_OUT_CMD     0x60

#define KBC_IN_CMD 0x64

#define KBC_ST_REG  0x64

#define KBC_CMD_REG 0x64

#define KBC_OBF BIT(0)

#define KBC_OUT_BUF 0x60

#define KBC_IN_BUF  0x60

#define KBC_PAR_ERR BIT(7)

#define KBC_TO_ERR BIT(6)

#define FULL_IN_BUF BIT(1)

#define KBC_READ_CMD 0x20

#define ENABLE_INT BIT(0)

#define BREAK_CODE_ESC 0x81

#define MAKE_CODE BIT(7)

#define TWO_BYTES 0xE0

#define IRQ_MOUSE 12

#define ACK 0xFA

#define MOUSE_WRITE_BYTE 0xD4

#define FIRST_BYTE BIT(3)

#define MOUSE_SIGNAL_X BIT(4)

#define MOUSE_SIGNAL_Y BIT(5)

#define MOUSE_OVERFLOW_X BIT(6)

#define MOUSE_OVERFLOW_Y BIT(7)

#define MOUSE_LB BIT(0)

#define MOUSE_RB BIT(1)

#define MOUSE_MB BIT(2)

#define ENABLE_DATA_REPORT 0xF4

#define DISABLE_DATA_REPORT 0xF5

#define MOUSE_READ_DATA 0xEB

#define ENABLE_STREAM_MODE 0xEA

#endif /* _LCOM_I8042_H_ */
