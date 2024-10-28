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


struct timeval
{
#ifdef __USE_TIME_BITS64
  __time64_t tv_sec;		/* Seconds.  */
  __suseconds64_t tv_usec;	/* Microseconds.  */
#else
  int tv_sec;		/* Seconds.  */
  int tv_usec;	/* Microseconds.  */
#endif
};


int sys_gettimeofday(struct timeval *tv) {
  // 获取当前时间
  tv->tv_sec = io_read(AM_TIMER_UPTIME).us / 1000000;
  panic("%d", tv->tv_sec);
  tv->tv_usec = io_read(AM_TIMER_UPTIME).us % 1000000;
  return 1;
}

int sys_close(int fd) {
  return 0;
}

size_t sys_lseek(int fd, int offset, int whence) {
  size_t ret = fs_lseek(fd, offset, whence);
  return ret;
}

int sys_read(int fd, void *buf, size_t count) {
  int ret = fs_read(fd, buf, count);
  if (ret < 0) panic("Read ERROR");
  return ret;
}

int sys_open(const char *path, int flags, int mode) {
  int ret = fs_open(path, 0, 0);
  if (ret == -1) panic("Open ERROR");
  return ret;
}

int sys_brk(void *addr) {
  return 0;  // 成功
}

int sys_write(int fd, char *buf, int len) {
  int ret = fs_write(fd, buf, len);
  if (ret < 0) panic("Write ERROR");
  return ret;
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
     case SYS_gettimeofday:
      ret = sys_gettimeofday((struct timeval *)a[1]);
      c->GPRx = ret;
      break;
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

