#include <stdio.h>
#include <NDL.h>

extern unsigned int NDL_GetTicks();

int main() {
    int tol_time = 500000;
    while (1) { 
        while (NDL_GetTicks() < tol_time);
        printf("time: %d\n", (int)tol_time / 500000);
        tol_time += 500000;
    }
    return 0;
}