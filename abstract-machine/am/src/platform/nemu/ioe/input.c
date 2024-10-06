#include <am.h>
#include <nemu.h>
#include </home/garlic/ics2023/abstract-machine/am/src/riscv/riscv.h>

#define KEYDOWN_MASK 0x8000

// static uint32_t keymap[256] = {};


void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  kbd->keydown = false;
  kbd->keycode = AM_KEY_NONE;

  uint8_t code = inb(KBD_ADDR) | KEYDOWN_MASK;
  if (code != AM_KEY_NONE) {
    kbd->keycode = code;
    kbd->keydown = true;
  }
}
