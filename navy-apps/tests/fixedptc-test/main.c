#include <stdio.h>
#include </home/garlic/ics2023/navy-apps/libs/libfixedptc/include/fixedptc.h>

int main() {
    // fixedpt fixedpt_muli
    fixedpt A1 = fixedpt_rconst(-1.8);
    fixedpt B1 = fixedpt_muli(A1, 3);
    printf("%d\n", fixedpt_toint(B1));  // -6
    // fixedpt fixedpt_divi(fixedpt A, int B)
    fixedpt A2 = fixedpt_rconst(6.8);
    fixedpt B2 = fixedpt_divi(A2, 3);
    printf("%d\n", fixedpt_toint(B2));  // 2
    // fixedpt fixedpt_mul(fixedpt A, fixedpt B)
    fixedpt A3 = fixedpt_rconst(7.8);
    fixedpt B3 = fixedpt_rconst(1.1);
    fixedpt C3 = fixedpt_mul(A3, B3);
    printf("%d\n", fixedpt_toint(C3));  // 8   
    // fixedpt fixedpt_div(fixedpt A, fixedpt B)
    fixedpt A4 = fixedpt_rconst(7.8);
    fixedpt B4 = fixedpt_rconst(1.1);
    fixedpt C4 = fixedpt_div(A4, B4);
    printf("%d\n", fixedpt_toint(C4));  // 7
    // fixedpt fixedpt_abs(fixedpt A)
    fixedpt A5 = fixedpt_rconst(7.8);
    fixedpt B5 = fixedpt_rconst(-6.9);
    fixedpt C5 = fixedpt_abs(A5);
    fixedpt D5 = fixedpt_abs(B5);
    printf("%d\n", fixedpt_toint(C5));  // 7    
    printf("%d\n", fixedpt_toint(D5));  // 7   
    // fixedpt fixedpt_floor(fixedpt A)
    fixedpt A6 = fixedpt_rconst(7.8);
    fixedpt B6 = fixedpt_rconst(-7.8);
    fixedpt C6 = fixedpt_floor(A6);
    fixedpt D6 = fixedpt_floor(B6);
    printf("%d\n", fixedpt_toint(C6));  // 7
    printf("%d\n", fixedpt_toint(D6));  // -8
    // fixedpt fixedpt_ceil(fixedpt A)
    fixedpt A7 = fixedpt_rconst(7.8);
    fixedpt B7 = fixedpt_rconst(-7.8);
    fixedpt C7 = fixedpt_ceil(A7);
    fixedpt D7 = fixedpt_ceil(B7);
    printf("%d\n", fixedpt_toint(C7));  // 8    
    printf("%d\n", fixedpt_toint(D7));  // -7
    return 0;
}