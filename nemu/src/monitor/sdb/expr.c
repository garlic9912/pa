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

#include <stdlib.h>
#include <string.h>
#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ,
  /* TODO: Add more token types */
  TK_PLUS, TK_MINUS, TK_MUL, TK_DIV,
  TK_LB, TK_RB, TK_NUM, TK_HEX,
  TK_REG
};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */
  {" +", TK_NOTYPE},    // spaces
  {"\\+", TK_PLUS},         // plus
  {"\\-", TK_MINUS},         // minus
  {"\\*", TK_MUL},         // mutiply
  {"\\/", TK_DIV},         // divide
  {"\\(", TK_LB},         // left brac
  {"\\)", TK_RB},         // right brac
  {"[0-9]+", TK_NUM},   // numbers (one or more digits)
  // {"0[xX][0-9a-fA-F]+", TK_HEX}, // hexadecimal numbers
  {"\\$[a-z0-9]+", TK_REG}, // registers
  {"==", TK_EQ},        // equal
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[128] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      // match successfully
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case(TK_PLUS):
          case(TK_MINUS):
          case(TK_MUL):
          case(TK_DIV):
          case(TK_LB):
          case(TK_RB):
            tokens[nr_token++].type = rules[i].token_type;
            break;       
          case(TK_NUM):
            tokens[nr_token].type = rules[i].token_type;
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            nr_token++;
            break;   
          case(TK_REG):
            tokens[nr_token].type = rules[i].token_type; 
            strncpy(tokens[nr_token].str, substr_start+1, substr_len-1);    
            tokens[nr_token].str[substr_len-1] = '\0';         
          default: 
            break;
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

static bool check_parentheses(int p, int q) {
  int match[16];
  int len_idx = 0;
  // p, q is LB and RB?
  if (tokens[p].type == TK_LB && tokens[q].type == TK_RB) {
    for (int i = p; i <= q; i++) {
      if (tokens[i].type == TK_LB) {
        match[len_idx++] = 1;
      } else if (tokens[i].type == TK_RB) {
        if (match[len_idx-1] == 1) {
          len_idx -= 1;
          if (len_idx == 0 && i == q) {
            return true;
          } else if (len_idx == 0 && i != q) {
            return false;
          } 
        }
      }
    }
  }
  return false;
}

int op_idx(int p, int q) {
  int tmp_RB;
  int idx[2] = {p, 2};
  for (int i = p; i <= q; i++) {
    // find the largest bracket and ignore it
    if (tokens[i].type == TK_LB) {
      tmp_RB = i+1;
      while (check_parentheses(i, tmp_RB) == false) {
        tmp_RB++;
      }
      i = tmp_RB;
      continue;
    }
    // find the op with lowest precedence
    if (tokens[i].type == TK_PLUS || tokens[i].type == TK_MINUS) {
      if (idx[1] >= 1) {
        idx[0] = i;
        idx[1] = 1;
      }
    } else if (tokens[i].type == TK_MUL || tokens[i].type == TK_DIV) {
      if (idx[1] == 2) {
        idx[0] = i;
        idx[1] = 2;
      }
    }
  }
  return idx[0];
}


static word_t eval(int p, int q) {
  if (p > q) {
    printf("eval p > q wrong\n");
    assert(0);
  }
  else if (p == q) {
    return atoi(tokens[p].str);
  }
  else if (check_parentheses(p, q) == true) {
    return eval(p + 1, q - 1);
  }
  else {
    int op = op_idx(p, q);
    word_t val1 = eval(p, op - 1);
    word_t val2 = eval(op + 1, q);

    switch (tokens[op].type) {
      case TK_PLUS: return val1 + val2;
      case TK_MINUS: return val1 - val2;
      case TK_MUL: return val1 * val2;
      case TK_DIV: return val1 / val2;
      default: assert(0);
    }
  }
  puts("There is a problem with the calculation of the given expression\n");
  assert(0);
  return 0;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  
  /* TODO: Insert codes to evaluate the expression. */
  return eval(0, nr_token-1);
}
