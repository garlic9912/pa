#include <am.h>
#include <nemu.h>
#include </home/garlic/ics2023/abstract-machine/am/src/riscv/riscv.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  kbd->keydown = false;
  kbd->keycode = AM_KEY_NONE;

  uint32_t code = (uint32_t)inl(KBD_ADDR);
  if (code != AM_KEY_NONE) {
    kbd->keycode = AM_KEY_Y | KEYDOWN_MASK;
    kbd->keydown = true;
  }
}
