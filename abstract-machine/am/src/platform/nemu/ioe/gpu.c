#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

void __am_gpu_init() {
  // int i;
  // int w = 400;  // TODO: get the correct width
  // int h = 300;  // TODO: get the correct height
  // uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // for (i = 0; i < w * h; i ++) fb[i] = i;
  // outl(SYNC_ADDR, 1);  
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  uint32_t w_h_config = inl(VGACTL_ADDR);
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = w_h_config >> 16, .height = w_h_config & 0xffff,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int i;
  // 获取宽度（从vga.c的初始化文件中可以得知）
  int w = inw(VGACTL_ADDR + 2);
  int h = inw(VGACTL_ADDR);
  // 获取显存起始地址
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // 向显存写入内容
  for (i = 0; i < w * h; i++) fb[i] = 0xFF000000; // 显存初始化，初始背景
  // 向同步寄存器写入1，让其更新屏幕
  outl(SYNC_ADDR, 1); 
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
