#include "KBC.h"

int (read_KBC_data)(uint8_t port, uint8_t *output, uint8_t mouse){

    uint8_t stat, data;
    uint8_t attempts=10;

    while (attempts){
        if (util_sys_inb(KBC_ST_REG, &stat)!=0){
            return 1;
        }
        if ((stat & KBC_OBF)!=0){
            if (util_sys_inb(port, &data)!=0){
                return 1;
            }
            if (mouse && !(stat & BIT(5))) {
                return 1;
            }
            if (!mouse && (stat & BIT(5))) {
                return 1;
            }
            if ((stat & BIT(7))!=0){
                return 1;
            }
            if ((stat & BIT(6))!=0){
                return 1;
            }
            if ((stat & (KBC_PAR_ERR | KBC_TO_ERR))==0){
                *output=data;
                return 0;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(20000));
        attempts--;
    }
    return 1;
}

int (write_KBC_data)(uint8_t port, uint8_t commandByte){

    uint8_t stat;
    uint8_t attempts=10;

    while (attempts){
        if (util_sys_inb(KBC_ST_REG, &stat)!=0){
            return 1;
        }
        if ((stat & FULL_IN_BUF)==0){
            if (sys_outb(port,commandByte)!=0){
                return 1;
            }
            return 0;
        }
        tickdelay(micros_to_ticks(DELAY_US));
        attempts--;
    }
    return 1;
}
