#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <BMP.h>
#include <NDL.h>


int main() {
  NDL_Init(0);
  int w, h;
  void *bmp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
  assert(bmp);
  NDL_OpenCanvas(&w, &h);
  // 更改参数实现画布居中
  NDL_DrawRect(bmp, 0, 0, w, h);
  free(bmp);
  NDL_Quit();
  printf("Test ends! Spinning...\n");
  while (1);
  return 0;
}
