#include <common.h>
#include "/home/garlic/ics2023/navy-apps/libs/libos/src/syscall.h"

#ifdef CONFIG_STRACE
static const char* syscall_names[] = {
  "SYS_exit",
  "SYS_yield",
  "SYS_open",
  "SYS_read",
  "SYS_write",
  "SYS_kill",
  "SYS_getpid",
  "SYS_close",
  "SYS_lseek",
  "SYS_brk",
  "SYS_fstat",
  "SYS_time",
  "SYS_signal",
  "SYS_execve",
  "SYS_fork",
  "SYS_link",
  "SYS_unlink",
  "SYS_wait",
  "SYS_times",
  "SYS_gettimeofday"
};
#endif


void sys_exit(int status) {
  halt(status);
}

int sys_yield() {
  yield();
  return 0;
}

void do_syscall(Context *c) {

  panic("1111111111111111111111111111");
  // 返回值，给strace使用
  int ret = 0;

  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  switch (a[0]) {
    case SYS_yield: 
      ret = sys_yield();
      c->GPRx = ret;
      break;
    case SYS_exit:
      sys_exit(a[1]);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

#ifdef CONFIG_STRACE
  printf("%s:(%d, %d, %d), return: %d\n", syscall_names[a[0]], a[1], a[2], a[3], ret);
#endif

}

