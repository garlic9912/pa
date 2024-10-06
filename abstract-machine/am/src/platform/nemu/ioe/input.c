#include <am.h>
#include <nemu.h>
#include </home/garlic/ics2023/abstract-machine/am/src/riscv/riscv.h>

#define KEYDOWN_MASK 0x8000

// #define SDL_KEYMAP(k) keymap[AM_KEY_ ## k] = AM_KEY_ ## k;
// static uint32_t keymap[256] = {};

// static void init_keymap() {
//   AM_KEYS(SDL_KEYMAP);
// }


void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  kbd->keydown = false;
  kbd->keycode = AM_KEY_NONE;

  uint32_t code = (uint32_t)inl(KBD_ADDR);
  if (code == AM_KEY_NONE) panic("11111111111");
  if (code != AM_KEY_NONE) {
    kbd->keycode = AM_KEY_Y;
    kbd->keydown = true;
  }
}
