#include "KBC.h"

int (read_KBC_data)(uint8_t port, uint8_t *output, uint8_t mouse){

    uint8_t status;
    uint8_t attempts=10;

    while (attempts){
        if (util_sys_inb(KBC_ST_REG, &status)!=0){
            return 1;
        }
        if ((status & BIT(0)) != 0) {                       
            if(util_sys_inb(port, output) != 0){         
                printf("Error: Could not read output!\n");
                return 1;
            }
            if((status & BIT(7)) != 0){                    
                printf("Error: Parity error!\n");           
                return 1;
            }
            if((status & BIT(6)) != 0){                     // verifica erro de timeout
                printf("Error: Timeout error!\n");          // se existir, descarta
                return 1;
            }
            if (mouse && !(status & BIT(5))) {              // está à espera do output do rato
                printf("Error: Mouse output not found\n");  // mas o output não é do rato
                return 1;
            } 
            if (!mouse && (status & BIT(5))) {                 // está à espera do output do teclado
                printf("Error: Keyboard output not found\n"); // mas o output não é do teclado
                return 1;
            } 
            return 0; // sucesso: output correto lido sem erros de timeout ou de paridade
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
