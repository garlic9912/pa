#include <stdio.h>
#include <NDL.h>

extern int NDL_PollEvent(char *buf, int len);

int main() {
  NDL_Init(0);
  while (1) {
    char buf[64];
    if (NDL_PollEvent(buf, sizeof(buf))) {
      printf("receive event: %s\n", buf);
    }
  }
  return 0;
}
