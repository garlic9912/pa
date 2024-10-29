#include <stdio.h>
#include <assert.h>
#include <sys/time.h>

int main() {
    struct timeval time;
    int tol_time = 500000;
    while (1) { 
        while ((int)time.tv_sec * 1000000 + (int)time.tv_usec < tol_time) {
            gettimeofday(&time, NULL);
            // printf("%d\n", (int)time.tv_usec);
        } 
        printf("time: %d\n", (int)tol_time / 500000);
        tol_time += 500000;
    }
    return 0;
}