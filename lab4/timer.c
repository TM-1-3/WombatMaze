#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int timer_hook_id = 0;

int timerCounter=0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  if (freq < 19 || freq > TIMER_FREQ){
    return 1;
  }
  uint8_t timerControlWord=0;
  if (timer_get_conf(timer, &timerControlWord)!=0){
    return 1;
  }
  timerControlWord = TIMER_LSB_MSB | (0X0F & timerControlWord);
  uint32_t counterInitialValue = TIMER_FREQ / freq;
  uint8_t LSB;
  util_get_LSB(counterInitialValue, &LSB);
  uint8_t MSB;
  util_get_MSB(counterInitialValue, &MSB);
  uint8_t timerSelected;
  switch (timer){
    case 0:
      timerSelected=TIMER_0;
      timerControlWord |= TIMER_SEL0;
      break;
    case 1:
      timerSelected=TIMER_1;
      timerControlWord |= TIMER_SEL1;
      break;
    case 2:
      timerSelected=TIMER_2;
      timerControlWord |= TIMER_SEL2;
      break;
    default:
      return 1;
  }
  if (sys_outb(TIMER_CTRL, timerControlWord)!=0){
    return 1;
  }
  if (sys_outb(timerSelected,LSB)!=0){
    return 1;
  }
  if (sys_outb(timerSelected,MSB)!=0){
    return 1;
  }
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  if (bit_no == NULL){
    return 1;
  }
  *bit_no = BIT(timer_hook_id);
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id)!=0){
    return 1;
  }
  return 0;
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&timer_hook_id)!=0){
    return 1;
  }
  return 0;
}

void (timer_int_handler)() {
  timerCounter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  if (st == NULL || !(timer >= 0 && timer <= 2)){
    return 1;
  }
  uint8_t readBackCommand = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if (sys_outb(TIMER_CTRL, readBackCommand)!=0){
    return 1;
  }
  if (util_sys_inb(TIMER_0 + timer, st)){
    return 1;
  }
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st, enum timer_status_field field) {
  union timer_status_field_val timerConfig;
  switch (field){
    case tsf_all:
      timerConfig.byte=st;
      break;
    case tsf_initial:
      st = 0x03 & (st>>4);
      if (st==1){
        timerConfig.in_mode=LSB_only;
      }
      else if(st==2){
        timerConfig.in_mode=MSB_only;
      }
      else if(st==3){
        timerConfig.in_mode=MSB_after_LSB;
      }
      else{
        timerConfig.in_mode=INVAL_val;
      }
      break;
    case tsf_mode:
      st = 0x07 & (st>>1);
      if (st==6){
        timerConfig.count_mode=2;
      }
      else if(st==7){
        timerConfig.count_mode=3;
      }
      else{
        timerConfig.count_mode=st;
      }
      break;
    case tsf_base:
      timerConfig.bcd=TIMER_BCD & st;
      break;
    default:
      return 1;
  }
  if (timer_print_config(timer, field, timerConfig)!=0){
    return 1;
  }
  return 0;
}
