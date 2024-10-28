#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

int main() {
    struct timeval time;
    int utime = 500000;
    while (1) {
        gettimeofday(&time, NULL);
        // 现在一直卡在这里
        while (time.tv_usec < utime) {
            printf("%d\n", (int)time.tv_usec);
        } 
        printf("time: %d\n", (int)time.tv_usec);
        utime += 500000;
    }
    return 0;
}