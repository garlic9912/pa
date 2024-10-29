#include <common.h>
#include <fs.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

// 串口
size_t serial_write(const void *buf, size_t offset, size_t len) {
  for (int i = 0; i < len; i++) {
    putch(*((char *)buf + i));
  }
  return len;
}

// 事件写入，如按键的down和up
size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == 0) return 0;
  // panic("1111111111111111");
  // 假设一次只读出一个事件
  char ev_info[30];
  sprintf(ev_info, "Got  (kbd): %s (%d) %s", keyname[ev.keycode], ev.keycode, ev.keydown ? "DOWN" : "UP");
  memcpy((char *)buf, ev_info, strlen(ev_info));
  return strlen(ev_info);
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
