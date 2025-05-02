// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

#include "video.h"
#include "keyboard.h"
#include "timer.c"

extern uint8_t scanCode;
extern vbe_mode_info_t modeInfo;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(exit_ESC)(){

  uint8_t keyboard_irq_set;
  message msg;
  int ipc_status;

  if (keyboard_subscribe_int(&keyboard_irq_set)!=0){
    return 1;
  }
  while (scanCode!=BREAK_CODE_ESC){
    if (driver_receive(ANY,&msg,&ipc_status)!=0){
      continue;
    }
    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & keyboard_irq_set){
            kbc_ih();
            break;
          }
        default:
          break;
      }
    }
  }
  if (keyboard_unsubscribe_int()!=0){
    return 1;
  }
  return 0;
}

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color){
  for (unsigned i=0;i<height;i++){
    if (draw_horizontal_line(x,y+i,width,color)!=0){
      vg_exit();
      return 1;
    }
  }
  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (set_mode_graphic(mode)!=0){
    return 1;
  }
  build_frame_buffer(mode);
  sleep(delay);
  if (vg_exit()!=0){
    return 1;
  }
  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,uint16_t width, uint16_t height,uint32_t color){
  if (build_frame_buffer(mode)!=0){
    return 1;
  }
  if (set_mode_graphic(mode)!=0){
    return 1;
  }
  uint32_t new_color;
  if (normalize_color(color, &new_color) != 0){ 
    return 1;
  }
  if (draw_rectangle(x,y,width,height,new_color)!=0){
    return 1;
  }
  sleep(5);
  if (exit_ESC()!=0){
    return 1;
  }
  if (vg_exit()!=0){
    return 1;
  }
  return 0;
}

uint32_t (Red)(uint32_t first){
  return (first>>modeInfo.RedFieldPosition) & ((1<<modeInfo.RedMaskSize)-1);
}

uint32_t (Green)(uint32_t first){
  return (first>>modeInfo.GreenFieldPosition) & ((1<<modeInfo.GreenMaskSize)-1);
}

uint32_t (Blue)(uint32_t first){
  return (first>>modeInfo.BlueFieldPosition) & ((1<<modeInfo.BlueMaskSize)-1);
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if (build_frame_buffer(mode)!=0){
    return 1;
  }
  if (set_mode_graphic(mode)!=0){
    return 1;
  }
  int noRectanglesHorizontal = modeInfo.XResolution / no_rectangles;
  int noRectanglesVertical = modeInfo.YResolution / no_rectangles;
  for (int row=0; row<no_rectangles; row++){
    for (int col=0; col<no_rectangles;col++){
      uint32_t color;
      if (modeInfo.MemoryModel==DIRECT_MODE){
        uint32_t R = (Red(first)+col*step) % (1<<modeInfo.RedMaskSize);
        uint32_t G = (Green(first)+row*step) % (1<<modeInfo.GreenMaskSize);
        uint32_t B = (Blue(first)+(col+row)*step) % (1<<modeInfo.BlueMaskSize);
        color = (R<<modeInfo.RedFieldPosition) | (G<<modeInfo.GreenFieldPosition) | (B<<modeInfo.BlueFieldPosition);
      }
      else{
        color = (first+(row*no_rectangles+col)*step) % (1<<modeInfo.BitsPerPixel);
      }
      if (draw_rectangle(col*noRectanglesHorizontal, row*noRectanglesVertical, noRectanglesHorizontal, noRectanglesVertical, color)!=0){
        return 1;
      }
    }
  }
  sleep(5);
  if (exit_ESC()!=0){
    return 1;
  }
  if (vg_exit()!=0){
    return 1;
  }
  return 0;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if (build_frame_buffer(VBE_768p_INDEXED)!=0){
    return 1;
  }
  if (set_mode_graphic(VBE_768p_INDEXED)!=0){
    return 1;
  }
  if (draw_xpm(xpm,x,y)!=0){
    return 1;
  }
  sleep(5);
  if (exit_ESC()!=0){
    return 1;
  }
  if (vg_exit()!=0){
    return 1;
  }
  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {

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
  if (timer_set_frequency(0,fr_rate)!=0){
    return 1;
  }
  if (build_frame_buffer(VBE_768p_INDEXED)!=0){
    return 1;
  }
  if (set_mode_graphic(VBE_768p_INDEXED)!=0){
    return 1;
  }
  if (draw_xpm(xpm,xi,yi)!=0){
    return 1;
  }
  while (scanCode!=BREAK_CODE_ESC){
    if (driver_receive(ANY, &msg, &ipc_status)!=0){
      continue;
    }
    if (is_ipc_notify(ipc_status)){
      switch(_ENDPOINT_P(msg.m_source)){
        case HARDWARE:
          if (msg.m_notify.interrupts & keyboard_irq_set){
            kbc_ih();
          }
          if (msg.m_notify.interrupts & timer_irq_set){
            if (draw_rectangle(xi,yi,100,100,0xFF)!=0){
              return 1;
            }
            if (xi==xf){
              yi+=speed;
              yi=yf;
            }
            else{
              xi+=speed;
              xi=xf;
            }
            if (draw_xpm(xpm,xi,yi)!=0){
              return 1;
            }
          }
      }
    }
  }
  if (vg_exit()!=0){
    return 1;
  }
  if (timer_unsubscribe_int()!=0){
    return 1;
  }
  if (keyboard_unsubscribe_int()!=0){
    return 1;
  }
  return 0;
}

int(video_test_controller)() {
  /* This year you do not need to implement this */
  printf("%s(): under construction\n", __func__);

  return 1;
}
