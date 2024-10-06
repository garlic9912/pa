#include <am.h>
#include <nemu.h>
#include </home/garlic/ics2023/abstract-machine/am/src/riscv/riscv.h>

#define KEYDOWN_MASK 0x8000

static uint32_t keymap[256] = {
  AM_KEY_NONE,
  AM_KEYS(AM_KEY_NAMES)
};



void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  kbd->keydown = false;
  kbd->keycode = AM_KEY_NONE;

  uint8_t code = inb(KBD_ADDR);
  if (keymap[code] != AM_KEY_NONE) {
    kbd->keycode = keymap[code];
    kbd->keydown = true;
  }
}
