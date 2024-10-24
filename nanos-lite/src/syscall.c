#include <common.h>
#include "/home/garlic/ics2023/navy-apps/libs/libos/src/syscall.h"


int sys_yield() {
  yield();
  return 0;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPRx;

  switch (a[0]) {
    case SYS_yield: 
      sys_yield();
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}

