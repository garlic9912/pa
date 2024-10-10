#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

static int idx = 0;

void __am_gpu_init() {
  int i;
  int w = 400;  // TODO: get the correct width
  int h = 300;  // TODO: get the correct height
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);  
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
  int k = 0;
  if (ctl->pixels != NULL) {
    while (*((uint32_t *)(ctl->pixels) + k) != 0) {
      outl(FB_ADDR + idx*4, *((uint32_t *)(ctl->pixels) + k));
      k++;
    }
  }
  // sync
  if (ctl->sync) {
    outl(SYNC_ADDR, 1);
    idx = 0;
  }  
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
