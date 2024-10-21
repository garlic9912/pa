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

#include <isa.h>
#include <cpu/difftest.h>
#include </home/garlic/ics2023/nemu/src/monitor/sdb/sdb.h>



word_t isa_raise_intr(word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  // mepc <- pc
  cpu.csr[0x341] = epc;
  // mcause <- NO
  cpu.csr[0x342] = NO;
  // 处理程序的地址 mtvec
  word_t addr = cpu.csr[0x305];
IFDEF(CONFIG_ITRACE, printf("11111"));
  return addr;
}

word_t isa_query_intr() {
  return INTR_EMPTY;
}
