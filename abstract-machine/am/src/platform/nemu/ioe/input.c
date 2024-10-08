#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000


void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t code = inl(KBD_ADDR);
  if (code & KEYDOWN_MASK){ 
    kbd->keydown = true;
    kbd->keycode = code ^ KEYDOWN_MASK;
  }
  else {
    kbd->keydown = false;
    kbd->keycode = code;
  }
}
