#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

int main() {
    struct timeval time;
    int stime = 0;
    int utime = 500000;
    int tol_time = stime * 1000000 + utime;
    while (1) { 
        while ((int)time.tv_sec * 1000000 + (int)time.tv_usec < tol_time) {
            gettimeofday(&time, NULL);
            // printf("%d\n", (int)time.tv_usec);
        } 
        printf("time: %d\n", (int)stime);
        tol_time += 500000;
    }
    return 0;
}