/* PicoRuby compiler */
#include <picorbc.h>

/* C */
#include "gpio.h"
#include "usb.h"
#include "uart.h"
#include "rgb.h"
//TODO: #include "spi.h"

/* ruby */
#include "ruby/keyboard.c"
#include "ruby/keymap.c"
#include "ruby/tud.c"
#include "ruby/rgb.c"

void
c_board_millis(mrb_vm *vm, mrb_value *v, int argc)
{
  SET_INT_RETURN(board_millis());
}

#define MEMORY_SIZE (1024*180)

static uint8_t memory_pool[MEMORY_SIZE];

mrbc_tcb *tcb_rgb; /* from rgb.h */

int main() {
  stdio_init_all();
  board_init();
  tusb_init();
  mrbc_init(memory_pool, MEMORY_SIZE);
  mrbc_define_method(0, mrbc_class_object, "board_millis",  c_board_millis);
  GPIO_INIT();
  USB_INIT();
  UART_INIT();
  RGB_INIT();
  mrbc_create_task(keyboard, 0);
  mrbc_create_task(tud, 0);
  mrbc_create_task(keymap, 0);
  tcb_rgb = mrbc_create_task(rgb, 0);
  mrbc_run();
  return 0;
}
