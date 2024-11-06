#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


static int evtdev = -1;
static int fbdev = -1;





// 画布的宽和高
static int canvas_w, canvas_h;
// 屏幕的宽和高
static int screen_w = 0, screen_h = 0;


// 返回微秒数
uint32_t NDL_GetTicks() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return (int)tv.tv_sec * 1000000 + (int)tv.tv_usec;
}


// 读出一条事件信息, 将其写入`buf`中, 最长写入`len`字节
// 若读出了有效的事件, 函数返回1, 否则返回0
int NDL_PollEvent(char *buf, int len) {
  int fd = open("/dev/events", 0);
  int ret = read(fd, buf, len);
  return ret;
}



void get_screen_wh(char *buf, int len) {
  // width
  int i = 0;
  while(*(buf+i) != '\n') {
    if (*(buf+i) <= '9' && *(buf+i) >= '0') {
      screen_w = screen_w * 10 + *(buf+i) - '0';
    }
    i++;
  }
  // height
  i++;
  while (i < len) {
    if (*(buf+i) <= '9' && *(buf+i) >= '0') {
      screen_h = screen_h * 10 + *(buf+i) - '0';
    }
    i++;    
  }
}



void NDL_OpenCanvas(int *w, int *h) {
  // if (getenv("NWM_APP")) {
  //   int fbctl = 4;
  //   fbdev = 5;
  //   screen_w = *w; screen_h = *h;
  //   char buf[64];
  //   int len = sprintf(buf, "%d %d", screen_w, screen_h);
  //   // let NWM resize the window and create the frame buffer
  //   write(fbctl, buf, len);
  //   while (1) {
  //     // 3 = evtdev
  //     int nread = read(3, buf, sizeof(buf) - 1);
  //     if (nread <= 0) continue;
  //     buf[nread] = '\0';
  //     if (strcmp(buf, "mmap ok") == 0) break;
  //   }
  //   close(fbctl);
  // }
  
  // 画布的大小
  canvas_w = *w;
  canvas_h = *h;
  // 获取屏幕的大小
  char buf[64];
  int fd = open("/proc/dispinfo", 0);
  int len = read(fd, buf, 64);
  buf[len] = '\0';
  get_screen_wh(buf, len);
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  int fd = open("/dev/fd", 0);
  // 按行写入
  for (int i = y; i < h+y; i++) {
    lseek(fd, i*screen_w*4+x*4, SEEK_SET);
    write(fd, pixels+(i-y)*w, w*4);
    lseek(fd, 0, SEEK_SET);
  }
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  return 0;
}

void NDL_Quit() {
}
