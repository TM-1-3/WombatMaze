#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "game/wombat.h"
#include "video.h"
#include "keyboard.h"
#include "timer.h"
#include "video.h"
#include "assets/wombat/vomabte_moving_4.xpm"

volatile extern uint8_t scanCode;

int (main)(int argc, char *argv[]){
    // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();
  return 0;
}

int (proj_main_loop)(int argc, char *argv[]){

    uint8_t timer_irq_set;  
    uint8_t keyboard_irq_set;
    int ipc_status;
    message msg;

    if (timer_subscribe_int(&timer_irq_set)!=0){
        return 1;
    }
    if (keyboard_subscribe_int(&keyboard_irq_set)!=0){
        return 1;
    }
    if (build_frame_buffer(VBE_600p_DC)!=0){
        return 1;
    }
    if (set_mode_graphic(VBE_600p_DC)!=0){
        return 1;
    }
    Wombat* wombat = loadWombat(10,10,(xpm_map_t)vomabte_moving_4);
    if (drawWombat(wombat)!=0){
        return 1;
    }
    int moveDirection=0;
    while (scanCode!=BREAK_CODE_ESC){
      if (driver_receive(ANY, &msg, &ipc_status)!=0){
        continue;
      }
      if (is_ipc_notify(ipc_status)){
        switch(_ENDPOINT_P(msg.m_source)){
          case HARDWARE:
            if (msg.m_notify.interrupts & keyboard_irq_set){
              kbc_ih();
              moveDirection=moveHandler(scanCode);
            }
            if (msg.m_notify.interrupts & timer_irq_set){
              if (draw_rectangle(0,0,100,100,0x00)!=0){
                return 1;
              }
              if (moveDirection!=0){
                moveWombat(wombat, moveDirection);
              }
              if (drawWombat(wombat)!=0){
                return 1;
              }
            }
            break;
        }
      }
    }
    if (vg_exit()!=0){
        return 1;
    }
    return 0;
}
