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
  // 假设一次只读出一个事件
  char ev_info[64];
  int count = sprintf(ev_info, "Got  (kbd): %s %s", keyname[ev.keycode], ev.keydown ? "DOWN" : "UP");
  memcpy((char *)buf, ev_info, count);
  return count;
}


// 获取屏幕大小
size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T t = io_read(AM_GPU_CONFIG);
  return sprintf((char *)buf, "WIDTH:%d\nHEIGHT:%d", t.width, t.height);
}


static int flag = 0;
// 用于把buf中的len字节写到屏幕上offset处
size_t fb_write(const void *buf, size_t offset, size_t len) {
  // 解出x, y, w
  int x, y, w;
  // panic("%d", offset);
  x = (offset / 4) % 400;
  y = ((offset / 4) - x) / 400;
  if(++flag == 129) panic("%d", y);

  w = len / 4;
  io_write(AM_GPU_FBDRAW, x, y, (int *)buf, w, 1, true);
  return 0;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
