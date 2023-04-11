#include "key.h"

bool wait(unsigned), uart_print(const char*);
void init(void), toggle_LED(void), sw_reset(void);
void clear_LED(void);
void learn_init(void), learn_deinit(void);
void control_off(void);
bool LED_state(void);
void ir_send(char);
bool ir_sending(void);

char device, toggle;

namespace
{

enum MODE { CONTROL, LEARN } mode = CONTROL;
int shift_count, shift_on_count;

void set_mode(MODE m) {
  if (mode == m) return;
  mode = m;
  if (mode == CONTROL) {
    learn_deinit();
    clear_LED();
    shift_on_count = 0;
    char c = key_index(get_key());
    if (c < 12) device = c;
  }
  if (mode == LEARN) {
    control_off();
    learn_init();
  }
}

} //anonymous

int main(void) {
  init();
  key_init();
  while (wait(1786)) if (mode == LEARN) toggle_LED();
}

void poll(unsigned char) {
  key_scan();
  if (shift_count) if (!--shift_count) set_mode(LEARN);
  if (shift_on_count) if (!--shift_on_count) clear_LED();
  if ((mode == CONTROL) && !LED_state() && !ir_sending()) asm ("sleep");
}

void key_handler(char c) {
  if (c == (KEY_POWER | KEY_7)) sw_reset();
  if ((c == KEY_SHIFT) && (mode == CONTROL)) {
    shift_count = 3572;				// 2 seconds
    toggle_LED();
  }
  if (LED_state()) shift_on_count = 8930;	// 5 seconds
  char i = key_index(c);
  static char buffer[] = "dk\r\n";// buffer should last for background printing
  if ((mode == CONTROL) && (i < 11)) {
    buffer[0] = device + 'a';
    buffer[1] = LED_state() ? i + 'A' : i + 'a';
    uart_print(buffer);
    if (LED_state()) i += 11;
    ir_send(i);
  }
  if (c) set_mode(CONTROL);
  else shift_count = 0;
}
