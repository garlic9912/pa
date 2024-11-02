#include <stdio.h>
#include </home/garlic/ics2023/navy-apps/libs/libfixedptc/include/fixedptc.h>

int main() {
    // fixedpt fixedpt_muli
    fixedpt A1 = fixedpt_rconst(-1.8);
    fixedpt B1 = fixedpt_muli(A1, 3);
    printf("%d\n", fixedpt_toint(B1));
    // fixedpt fixedpt_divi(fixedpt A, int B)
    fixedpt A2 = fixedpt_rconst(-1.8);
    fixedpt B2 = fixedpt_divi(A2, 3);
    printf("%d\n", fixedpt_toint(B2));
    return 0;
}