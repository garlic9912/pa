#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

int main() {
    struct timeval time;
    int stime = 1;
    while (1) {
        while (time.tv_sec < stime) {
            gettimeofday(&time, NULL);
            // printf("%d\n", (int)time.tv_usec);
        } 
        printf("time: %d\n", (int)time.tv_usec);
        stime += 1;
    }
    return 0;
}