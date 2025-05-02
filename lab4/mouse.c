#include "mouse.h"

int mouse_hook_id=3;
uint8_t mouseByte;
uint8_t byteIndex;
uint8_t mouseBytes[3];
struct packet mousePacket;

int (mouse_subscribe_int)(uint8_t *bit_no){
    if (bit_no==NULL){
        return 1;
    }
    *bit_no=BIT(mouse_hook_id);
    if (sys_irqsetpolicy(IRQ_MOUSE, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id)!=0){
        return 1;
    }
    return 0;
}

int (mouse_unsubscribe_int)(){
    if (sys_irqrmpolicy(&mouse_hook_id)!=0){
        return 1;
    }
    return 0;
}

void (mouse_ih)(){
    if (read_KBC_data(KBC_OUT_CMD,&mouseByte,1)!=0){
        printf("Error reading KBC data");
    }
}

int (mouse_write_data)(uint8_t command){

    uint8_t attempts=10;
    uint8_t responseMouse;

    do {
        attempts--;
        if (write_KBC_data(KBC_IN_CMD, MOUSE_WRITE_BYTE)) return 1;
        if (write_KBC_data(KBC_OUT_CMD, command)) return 1;
        tickdelay(micros_to_ticks(DELAY_US));
        if (util_sys_inb(KBC_OUT_CMD, &responseMouse)) return 1;
        if (responseMouse == ACK) return 0;
      } while (responseMouse != ACK && attempts);
    return 1;
}

void (mouse_sync_bytes)(){
    if ((mouseByte & FIRST_BYTE) && byteIndex==0){
        mouseBytes[byteIndex]=mouseByte;
        byteIndex++;
    }
    else if (byteIndex>0){
        mouseBytes[byteIndex]=mouseByte;
        byteIndex++;
    }
}

void (mouse_build_packet)(){
    for (int i=0; i<3; i++){
        mousePacket.bytes[i]=mouseBytes[i];
    }
    mousePacket.delta_x=(MOUSE_SIGNAL_X & mouseBytes[0]) ? (0xFF00 | mouseBytes[1]) : mouseBytes[1];
    mousePacket.delta_y=(MOUSE_SIGNAL_Y & mouseBytes[0]) ? (0xFF00 | mouseBytes[2]) : mouseBytes[2];
    mousePacket.x_ov=MOUSE_OVERFLOW_X & mouseBytes[0];
    mousePacket.y_ov=MOUSE_OVERFLOW_Y & mouseBytes[0];
    mousePacket.lb=MOUSE_LB & mouseBytes[0];
    mousePacket.rb=MOUSE_RB & mouseBytes[0];
    mousePacket.mb=MOUSE_MB & mouseBytes[0];
}
