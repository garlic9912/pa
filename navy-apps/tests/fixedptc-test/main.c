#include <stdio.h>
#include </home/garlic/ics2023/navy-apps/libs/libfixedptc/include/fixedptc.h>

int main() {
    fixedpt A = fixedpt_rconst(1.8);
    fixedpt B = fixedpt_muli(A, 3);
    printf("%d\n", 3*1.8);
    printf("%d\n", fixedpt_toint(B));
    return 0;
}