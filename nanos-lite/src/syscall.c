#include <common.h>
#include </home/garlic/ics2023/navy-apps/libs/libos/src/syscall.h>


void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPRx;

  switch (a[0]) {
    case SYS_yield: 
      // _yield();
      printf("%d\n", SYS_yield);
      panic("78787878");
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
