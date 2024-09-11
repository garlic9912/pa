/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static int idx = 0;
static int flag = 0;
static int depth = 0;
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  long result = %s; "
"  printf(\"%%ld\", result); "
"  return 0; "
"}";

static int choose(int n) {
  return rand() % n;
}


static void gen_num() {
  int tmp[3];
  int len = 0;
  int num = rand() % 1000 + 1;
  while (num != 0) {
    int x = num % 10;
    tmp[len++] = (char)(x + '0');
    num /= 10;
  }
  for (int i = len-1; i >= 0; i--) {
    if (idx+1 <= 120){
      buf[idx++] = tmp[i];
    } else {
      flag = 1;
      return;
    }
  }
}


static void gen(char c) {
  if (idx+1 <= 120){
    buf[idx++] = c;
  } else {
    flag = 1;
    return;
  }  
}


static void gen_rand_op() { 
  if (idx+1 <= 120){
    switch(choose(4)) {
      case 0: buf[idx++] = '+'; break;
      case 1: buf[idx++] = '-'; break;
      case 2: buf[idx++] = '*'; break;
      case 3: buf[idx++] = '/'; break;
    }
  } else {
    flag = 1;
    return;
  } 
}



static void gen_rand_expr() {
  if (++depth > 30) {
    flag = 1;
    return;
  }
  switch (choose(3)) {
    case 0: gen_num(); break;
    case 1: gen('('); gen_rand_expr(); gen(')'); break;
    default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }  
  buf[idx] = '\0';
}


int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    if (flag == 1) return 0;

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    long result;
    ret = fscanf(fp, "%ld", &result);
    pclose(fp);

    printf("%ld %s\n", result, buf);
  }
  return 0;
}
