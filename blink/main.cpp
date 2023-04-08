bool wait(unsigned), read_button(void), sw_reset(void);
void toggle_LED(void), init(void);

int main(void) {
  init();
  while (wait(1786)) toggle_LED();
}

void poll(unsigned char) {
  if (!read_button()) sw_reset();
}
