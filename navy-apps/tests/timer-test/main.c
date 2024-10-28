#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

int main() {
    struct timeval time;
    gettimeofday(&time, NULL);
    int utime = 500000;
    while (1) {
        
        // 现在一直卡在这里
        while (time.tv_usec < utime) {
            printf("%d\n", (int)time.tv_usec);
        } 
        printf("time: %d\n", (int)time.tv_usec);
        utime += 500000;
    }
    return 0;
}