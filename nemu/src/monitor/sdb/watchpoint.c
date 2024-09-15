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

#include "sdb.h"

#define NR_WP 32


static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

// Returns a free watchpoint structure from the free_ list
WP* new_wp() {
  WP* ret = NULL;
  if (free_ == NULL) {
    puts("there is no free wp\n");
    assert(0);
  }
  // remove wp form free_
  ret = free_;
  free_ = free_->next;
  // add wp to head
  ret->next = head->next;
  head = ret;

  return ret;
}


bool check() {
  if (head != NULL) {
    bool a = true;
    WP *wp = head;
    while (wp != NULL) {
      word_t now_val = expr(wp->expression, &a);
      if (now_val != wp->last_val) {
        puts("The program stopped because the monitored variable changed.\n");
        printf("watch point %d: %s\n", wp->NO, wp->expression);
        printf("last value: %d\n", wp->last_val);
        printf("now value: %d\n", now_val);
        return true;
      }
      wp = wp->next;
    }
  } 
  return false; 
}


// Return wp to the free_ list
void free_wp(WP *wp) {
  if (wp == NULL) {
    puts("the given wp is NULL error");
    assert(0);
  }
  wp->next = free_->next;
  free_ = wp; 
}

