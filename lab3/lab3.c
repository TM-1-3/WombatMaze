#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "i8254.h"
#include "keyboard.h"
#include "KBC.c"
#include "timer.c"

extern uint32_t KBCCounter;
extern uint32_t timerCounter;
extern uint8_t scanCode;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() {
  
  uint8_t irq_set;
  int ipc_status;
  message msg;

  if (keyboard_subscribe_int(&irq_set)!=0){
    return 1;
  }
  while (scanCode!=BREAK_CODE_ESC){
    if (driver_receive(ANY,&msg,&ipc_status)!=0){
      continue;
    }
    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set){
            kbc_ih();
            kbd_print_scancode(!(MAKE_CODE & scanCode), scanCode == TWO_BYTES ? 2 : 1, &scanCode);
          }
      }
    }
  }
  if (keyboard_unsubscribe_int()!=0){
    return 1;
  }
  if (kbd_print_no_sysinb(KBCCounter)!=0){
    return 1;
  }
  return 0;
}

int(kbd_test_poll)() {
  while (scanCode!=BREAK_CODE_ESC){
    if (read_KBC_data(KBC_OUT_CMD, &scanCode, 0)==0){
      kbd_print_scancode(!(MAKE_CODE & scanCode), scanCode==TWO_BYTES ? 2 : 1, &scanCode);
    }
  }
  return keyboard_restore_int();
}

int(kbd_test_timed_scan)(uint8_t n) {

  uint8_t timer_irq_set;
  uint8_t KBC_irq_set;
  int ipc_status;
  message msg;

  int timerSeconds=0;

  if (timer_subscribe_int(&timer_irq_set)!=0){
    return 1;
  }
  if (keyboard_subscribe_int(&KBC_irq_set)!=0){
    return 1;
  }
  while (timerSeconds<n && scanCode!=BREAK_CODE_ESC){
    if (driver_receive(ANY, &msg, &ipc_status)!=0){
      continue;
    }
    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & KBC_irq_set){
            kbc_ih();
            kbd_print_scancode(!(MAKE_CODE & scanCode), scanCode==TWO_BYTES ? 2:1, &scanCode);
            timerSeconds=0;
            timerCounter=0;
          }
          if (msg.m_notify.interrupts & timer_irq_set){
            timer_int_handler();
            if (timerCounter%60==0){
              timerSeconds++;
            }
          }
      }
    }
  }
  if (timer_unsubscribe_int()!=0){
    return 1;
  }
  if (keyboard_unsubscribe_int()!=0){
    return 1;
  }
  if (kbd_print_no_sysinb(KBCCounter)!=0){
    return 1;
  }
  return 0;
}
