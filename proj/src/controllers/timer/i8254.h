#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>    /* for BIT macro */

/* Clock frequency */
#define TIMER_FREQ    1193182
/* Timer IRQ */
#define TIMER0_IRQ    0

/* Port addresses */
#define TIMER_0       0x40  /* Timer 0 count register */
#define TIMER_1       0x41  /* Timer 1 count register */
#define TIMER_2       0x42  /* Timer 2 count register */
#define TIMER_CTRL    0x43  /* Control register        */
#define SPEAKER_CTRL  0x61  /* Register for speaker control */

/* Timer selection */
#define TIMER_SEL0    0x00      /* Select Timer 0 */
#define TIMER_SEL1    BIT(6)    /* Select Timer 1 */
#define TIMER_SEL2    BIT(7)    /* Select Timer 2 */
#define TIMER_RB  (BIT(7) | BIT(6))  /* Read-back command */

/* Register selection */
#define TIMER_LSB     BIT(4)    /* Initialize counter LSB only */
#define TIMER_MSB     BIT(5)    /* Initialize counter MSB only */
#define TIMER_BOTH (TIMER_LSB | TIMER_MSB)  /* Initialize LSB then MSB */

/* Operating modes */
#define TIMER_MODE0   0x00      /* Interrupt on terminal count */
#define TIMER_MODE1   BIT(1)    /* Hardware re-triggerable one-shot */
#define TIMER_MODE2   BIT(2)    /* Rate generator */
#define TIMER_MODE3   (BIT(2) | BIT(1))  /* Square wave generator */
#define TIMER_MODE4   (BIT(3) | BIT(1))  /* Software triggered strobe */
#define TIMER_MODE5   BIT(3)    /* Hardware triggered strobe */

/* Counting mode */
#define TIMER_BIN     0x00      /* Binary counting */
#define TIMER_BCD     0x01      /* BCD counting    */

/* Read-back flags */
#define RB_COUNT  BIT(5)          /* Read-back: count */
#define RB_STATUS BIT(4)          /* Read-back: status */
#define RB_SEL(n)   BIT((n) + 1)   /* Read-back: select timer n (0..2) */

#endif /* _LCOM_I8254_H_ */
