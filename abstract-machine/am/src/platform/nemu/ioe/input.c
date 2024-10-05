#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  kbd->keydown = false;
  kbd->keycode = AM_KEY_NONE;
  
  int key_code = (int)inl(KBD_ADDR);
  if (key_code != AM_KEY_NONE) {
    kbd->keydown = true;
    kbd->keycode = key_code;
  }
}
