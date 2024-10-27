#include <common.h>
#include "/home/garlic/ics2023/navy-apps/libs/libos/src/syscall.h"
#include <fs.h>

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

int sys_close(int fd) {
  return 0;
}

int sys_lseek(int fd, int offset, int whence) {
  return fs_lseek(fd, offset, whence);
}

int sys_read(int fd, void *buf, size_t count) {
  return fs_read(fd, buf, count);
}

int sys_open(const char *path, int flags, int mode) {
  return fs_open(path, 0, 0);
}

int sys_brk(void *addr) {
  return 0;  // 成功
}

int sys_write(int fd, char *buf, int len) {
  return fs_write(fd, buf, len);
}

void sys_exit(int status) {
  halt(status);
}

int sys_yield() {
  yield();
  return 0;
}




void do_syscall(Context *c) {
  // 返回值，给strace使用
  int ret = 0;

  uintptr_t a[4];
  a[0] = c->GPR1;  // type
  a[1] = c->GPR2;  // 参数1
  a[2] = c->GPR3;  // 参数2
  a[3] = c->GPR4;  // 参数3

  switch (a[0]) {
     case SYS_close:
      ret = sys_close(a[1]);
      c->GPRx = ret;
      break;    
    case SYS_lseek:
      ret = sys_lseek(a[1], a[2], a[3]);
      c->GPRx = ret;
      break;    
    case SYS_read:
      ret = sys_read(a[1], (char *)a[2], a[3]);
      c->GPRx = ret;
      break;     
    case SYS_open:
      ret = sys_open((char *)a[1], a[2], a[3]);
      c->GPRx = ret;
      break;      
    case SYS_brk:
      ret = sys_brk((void *)a[1]);
      c->GPRx = ret;
      break;
    case SYS_write:
      ret = sys_write(a[1], (char *)a[2], a[3]);
      c->GPRx = ret;
      break;
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

