#include <common.h>
#include "/home/garlic/ics2023/navy-apps/libs/libos/src/syscall.h"


void sys_exit(int status) {
  halt(status);
}

int sys_yield() {
  yield();
  return 0;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;

  switch (a[0]) {
    case SYS_yield: 
      c->GPRx = sys_yield();
      break;
    case SYS_exit:
      sys_exit(0);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

