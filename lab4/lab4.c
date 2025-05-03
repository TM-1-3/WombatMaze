// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "i8042.h"
#include "i8254.h"
#include "KBC.c"
#include "mouse.h"
#include "timer.c"

extern uint8_t byteIndex;
extern struct packet mousePacket;
extern int timerCounter;

typedef enum{
    START,
    UP,
    DOWN,
    VERTEX,
    END,
} State;

State state=START;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {
    
    uint8_t irq_set;
    int ipc_status;
    message msg;

    if (mouse_subscribe_int(&irq_set)!=0){
        return 1;
    }
    if (mouse_write_data(ENABLE_DATA_REPORT)!=0){
        return 1;
    }
    while (cnt){
        if (driver_receive(ANY, &msg, &ipc_status)!=0){
            continue;
        }
        if (is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                  if (msg.m_notify.interrupts & irq_set){
                    mouse_ih();
                    mouse_sync_bytes();
                    if (byteIndex==3){
                        mouse_build_packet();
                        mouse_print_packet(&mousePacket);
                        byteIndex=0;
                        cnt--;
                    }
                  }
                  break;
            }
        }
    }
    if (mouse_write_data(DISABLE_DATA_REPORT)!=0){
        return 1;
    }
    if (mouse_unsubscribe_int()!=0){
        return 1;
    }
    return 0;
}

int (mouse_test_async)(uint8_t idle_time) {
    
    uint8_t mouse_irq_set=0;
    uint8_t timer_irq_set=0;
    int ipc_status;
    message msg;
    uint8_t secondsPassed=0;
    uint16_t timerFrequency=sys_hz();

    if (mouse_subscribe_int(&mouse_irq_set)!=0){
        return 1;
    }
    if (timer_subscribe_int(&timer_irq_set)!=0){
        return 1;
    }
    if (mouse_write_data(ENABLE_DATA_REPORT)!=0){
        return 1;
    }
    while (secondsPassed<idle_time){
        if (driver_receive(ANY, &msg, &ipc_status)!=0){
            continue;
        }
        if (is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                  if (msg.m_notify.interrupts & timer_irq_set){
                    timer_int_handler();
                    if (timerCounter % timerFrequency==0){
                        secondsPassed++;
                    }
                  }
                  if (msg.m_notify.interrupts & mouse_irq_set){
                    mouse_ih();
                    mouse_sync_bytes();
                    if (byteIndex==3){
                        mouse_build_packet();
                        mouse_print_packet(&mousePacket);
                        byteIndex=0;
                    }
                    timerCounter=0;
                    secondsPassed=0;
                  }
            }
        }
    }
    if (mouse_write_data(DISABLE_DATA_REPORT)!=0){
        return 1;
    }
    if (timer_unsubscribe_int()!=0){
        return 1;
    }
    if (mouse_unsubscribe_int()!=0){
        return 1;
    }
    return 0;
}

void (update_state_machine)(uint8_t tolerance) {

    switch (state) {

      case START:

          // transição I
          // se só o left-button estiver pressionado
          if (mousePacket.lb && !mousePacket.rb && !mousePacket.mb) {
            state = UP;
          }

          break;

      case UP:
          //TODO: transições II, III e F
          break;

      case VERTEX:
          //TODO: transições IV e F
          break;

      case DOWN:
          //TODO: transições V, VI e F
          break;

      case END:
          break;
    }

    // Atualização do valor percorrido em X
    // x_len_total = max(0, x_len_total + mouse_packet.delta_x);
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    
    uint8_t irq_set;
    message msg;
    int ipc_status;

    if (mouse_subscribe_int(&irq_set)!=0){
        return 1;
    }
    if (mouse_write_data(ENABLE_DATA_REPORT)!=0){
        return 1;
    }
    while (state!=END){
        if (driver_receive(ANY, &msg, &ipc_status)!=0){
            continue;
        }
        if (is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                   if (msg.m_notify.interrupts & irq_set){
                        mouse_ih();
                        mouse_sync_bytes();
                        if (byteIndex==3){
                            mouse_build_packet();
                            //if (state==START){
                                //if (mousePacket.lb && !mousePacket.rb && !mousePacket.mb){
                                    //state=UP;
                                //}
                            //}
                            update_state_machine(tolerance);
                            byteIndex=0;
                    
                                
                        }
                    }
            }
        }
    }
    if (mouse_write_data(DISABLE_DATA_REPORT)!=0){
        return 1;
    }
    if (mouse_unsubscribe_int()!=0){
        return 1;
    }
    return 0;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
    while (cnt){
        if (mouse_write_data(MOUSE_READ_DATA)!=0){
            return 1;
        }
        mouse_ih();
        mouse_sync_bytes();
        if (byteIndex==3){
            mouse_build_packet();
            mouse_print_packet(&mousePacket);
            byteIndex=0;
            cnt--;
            tickdelay(micros_to_ticks(1000*period));
        }
    }
    if (mouse_write_data(ENABLE_STREAM_MODE)!=0){
        return 1;
    }
    if (mouse_write_data(DISABLE_DATA_REPORT)!=0){
        return 1;
    }
    uint8_t commandByte=minix_get_dflt_kbc_cmd_byte();
    if (write_KBC_data(KBC_IN_CMD,KBC_OUT_CMD)!=0){
        return 1;
    }
    if (write_KBC_data(KBC_OUT_CMD,commandByte)!=0){
        return 1;
    }
    return 0;
}
