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

#include <cpu/cpu.h>
#include <cpu/decode.h>
#include <cpu/difftest.h>
#include <locale.h>
#include </home/garlic/ics2023/nemu/src/monitor/sdb/sdb.h>


/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INST_TO_PRINT 10

CPU_state cpu = {};
uint64_t g_nr_guest_inst = 0;
static uint64_t g_timer = 0; // unit: us
static bool g_print_step = false;
Decode s;
extern char *elf_file;
int flag = 1;

void device_update();


#ifdef CONFIG_IRINGTRACE
static char ringbuf[10][128];
static int idx = 0;
static int flag = 0;
#endif


#ifdef CONFIG_FTRACE
// ELF constants and types
#define EI_NIDENT 16
#define SHN_UNDEF 0
#define PT_LOAD 1
#define SHT_STRTAB 3
#define SHT_SYMTAB 2
#define ELFCLASS32 1
#define ELFCLASS64 2

// ELF header structure definitions
typedef struct {
    unsigned char e_ident[EI_NIDENT]; // ELF file identification
    uint16_t e_type;                  // File type
    uint16_t e_machine;               // Machine type
    uint32_t e_version;               // File version
    uint32_t e_entry;                 // Entry point address
    uint32_t e_phoff;                 // Program header table offset
    uint32_t e_shoff;                 // Section header table offset
    uint32_t e_flags;                 // Processor-specific flags
    uint16_t e_ehsize;                // ELF header size in bytes
    uint16_t e_phentsize;             // Program header entry size
    uint16_t e_phnum;                 // Number of entries in program header table
    uint16_t e_shentsize;             // Section header entry size
    uint16_t e_shnum;                 // Number of entries in section header table
    uint16_t e_shstrndx;              // Section name string table index
} Elf32_Ehdr;

typedef struct {
    uint32_t sh_name;                 // Section name (index in string tbl)
    uint32_t sh_type;                 // Section type
    uint32_t sh_flags;                // Section flags
    uint32_t sh_addr;                 // Section virtual addr at execution
    uint32_t sh_offset;               // Section file offset
    uint32_t sh_size;                 // Section size in bytes
    uint32_t sh_link;                 // Link to another section
    uint32_t sh_info;                 // Additional information
    uint32_t sh_addralign;            // Section alignment
    uint32_t sh_entsize;              // Entry size if section holds table
} Elf32_Shdr;

typedef struct {
    uint32_t st_name;                 // Symbol name (index in string tbl)
    uint32_t st_value;                // Value of the symbol
    uint32_t st_size;                 // Size of the symbol
    unsigned char st_info;            // Information about the symbol
    unsigned char st_other;           // Other information
    uint16_t st_shndx;                // Section index
} Elf32_Sym;





// pc is next position
void ftrace(word_t pc) {
  // acquire the infomation of elf
  FILE *fp = NULL;
  Elf32_Shdr *shdr;
  Elf32_Sym *symtab;
  char *buf;
  char *strtab;
  int symtab_idx, strtab_idx;
  union {
    Elf32_Ehdr ehdr;
  } elf_hdr;
  // open the file
  if ((fp = fopen(elf_file, "rb")) == NULL) {
      panic("Cannot open file");
  } 

  // Seek to the end of the file to get its size
  fseek(fp, 0, SEEK_END);
  long fsize = ftell(fp);
  rewind(fp);
    
  // Allocate memory for the whole file
  buf = (char *)malloc(fsize);

  // Read the whole file into the buffer
  if (fread(buf, 1, fsize, fp) != fsize) {
      panic("Error reading file");
      free(buf);
      fclose(fp);
  }    

  elf_hdr.ehdr = *(Elf32_Ehdr *)buf;

  // Find the section header table
  printf("%d,   %d", elf_hdr.ehdr.e_phoff, elf_hdr.ehdr.e_shoff);
  panic("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  shdr = (Elf32_Shdr *)((char *)buf + elf_hdr.ehdr.e_shoff);

  // Find the symbol table and string table indexes
  symtab_idx = strtab_idx = -1;
  for (int i = 0; i < elf_hdr.ehdr.e_shnum; ++i) {
    if (shdr[i].sh_type == SHT_SYMTAB) {
      symtab_idx = i;
    }
    if (shdr[i].sh_type == SHT_STRTAB && i == elf_hdr.ehdr.e_shstrndx) {
      strtab_idx = i;
    }
  }

  // Get the symbol table and string table addresses
  symtab = (Elf32_Sym *)((char *)buf + shdr[symtab_idx].sh_offset);
  strtab = (char *)((char *)buf + shdr[strtab_idx].sh_offset);

  // Print symbols
  for (int i = 0; i < shdr[symtab_idx].sh_size / sizeof(Elf32_Sym); ++i) {
    const char *name = strtab + symtab[i].st_name;
    printf("Symbol: %s, Value: 0x%08X, Size: %u\n", name, symtab[i].st_value, symtab[i].st_size);
  }

  // Free allocated memory and close the file
  free(buf);
  fclose(fp);
}
#endif




static void trace_and_difftest(Decode *_this, vaddr_t dnpc) {
#ifdef CONFIG_IRINGTRACE
  // update the idx of ringbuf
  if (idx == 10) { idx = 0; flag = 1;}  
  strcpy(ringbuf[idx], "");
  char *p_ring = ringbuf[idx];
  p_ring += sprintf(p_ring, "   ");
  strcpy(p_ring, _this->logbuf);
  idx++;
#endif

#ifdef CONFIG_ITRACE_COND
  if (ITRACE_COND) { log_write("%s\n", _this->logbuf); }
#endif
  if (g_print_step) { 
    IFDEF(CONFIG_ITRACE, puts(_this->logbuf));  
  }
  IFDEF(CONFIG_DIFFTEST, difftest_step(_this->pc, dnpc));

// watch point function
#ifdef CONFIG_WATCHPOINT
  if (checkwp() == true) {
    nemu_state.state = NEMU_STOP;
  }
#endif  
}

static void exec_once(Decode *s, vaddr_t pc) {
  s->pc = pc;
  s->snpc = pc;

  isa_exec_once(s);
  cpu.pc = s->dnpc;

#ifdef CONFIG_FTRACE
  if (flag-- > 0) ftrace(cpu.pc);
#endif

#ifdef CONFIG_ITRACE
  char *p = s->logbuf;
  // 0x80000000:( 00 00 02 97 auipc   t0, 0x0)
  p += snprintf(p, sizeof(s->logbuf), FMT_WORD ":", s->pc); 
  int ilen = s->snpc - s->pc;
  // (0x80000000:) 00 00 02 97 (auipc   t0, 0x0)
  int i;
  uint8_t *inst = (uint8_t *)&s->isa.inst.val;
  for (i = ilen - 1; i >= 0; i --) {
    p += snprintf(p, 4, " %02x", inst[i]);
  }
  int ilen_max = MUXDEF(CONFIG_ISA_x86, 8, 4);
  int space_len = ilen_max - ilen;
  if (space_len < 0) space_len = 0;
  space_len = space_len * 3 + 1;
  memset(p, ' ', space_len);
  p += space_len;

#ifndef CONFIG_ISA_loongarch32r
  void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
  // (0x80000000: 00 00 02 97) auipc   t0, 0x0
  disassemble(p, s->logbuf + sizeof(s->logbuf) - p,
      MUXDEF(CONFIG_ISA_x86, s->snpc, s->pc), (uint8_t *)&s->isa.inst.val, ilen);
#else
  p[0] = '\0'; // the upstream llvm does not support loongarch32r
#endif
#endif
}

static void execute(uint64_t n) {
  for (;n > 0; n --) {
    exec_once(&s, cpu.pc);
    g_nr_guest_inst ++;
    trace_and_difftest(&s, cpu.pc);
    if (nemu_state.state != NEMU_RUNNING) break;
    IFDEF(CONFIG_DEVICE, device_update());
  }
}

static void statistic() {
  IFNDEF(CONFIG_TARGET_AM, setlocale(LC_NUMERIC, ""));
#define NUMBERIC_FMT MUXDEF(CONFIG_TARGET_AM, "%", "%'") PRIu64
  Log("host time spent = " NUMBERIC_FMT " us", g_timer);
  Log("total guest instructions = " NUMBERIC_FMT, g_nr_guest_inst);
  if (g_timer > 0) Log("simulation frequency = " NUMBERIC_FMT " inst/s", g_nr_guest_inst * 1000000 / g_timer);
  else Log("Finish running in less than 1 us and can not calculate the simulation frequency");
}

void assert_fail_msg() {
  isa_reg_display();
  statistic();
#ifdef CONFIG_IRINGTRACE
  Decode *ts = &s;
  if (idx == 10) {
    idx = 0;
    flag = 1;
  }  
  char *p_ring = ringbuf[idx];
  p_ring += sprintf(p_ring, "==>");
  p_ring += snprintf(p_ring, sizeof(s.logbuf), FMT_WORD ":",ts->pc);
  int ilen = ts->snpc - ts->pc;
  int i;
  uint8_t *inst = (uint8_t *)&ts->isa.inst.val;
  for (i = ilen - 1; i >= 0; i --) {
    p_ring += snprintf(p_ring, 4, " %02x", inst[i]);
  }
  int ilen_max = MUXDEF(CONFIG_ISA_x86, 8, 4);
  int space_len = ilen_max - ilen;
  if (space_len < 0) space_len = 0;
  space_len = space_len * 3 + 1;
  memset(p_ring, ' ', space_len);
  p_ring += space_len;
  void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
  disassemble(p_ring, ringbuf[idx] + sizeof(ts->logbuf) - p_ring-3,
      MUXDEF(CONFIG_ISA_x86, ts->snpc, ts->pc), (uint8_t *)&ts->isa.inst.val, ilen);
  idx++;

  if (flag == 1) idx = 10;
  for (int i = 0; i < idx; i++) {
    puts(ringbuf[i]);
  }
#endif
}

/* Simulate how the CPU works. */
void cpu_exec(uint64_t n) {
  g_print_step = (n < MAX_INST_TO_PRINT);
  switch (nemu_state.state) {
    case NEMU_END: case NEMU_ABORT:
      printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
      return;
    default: nemu_state.state = NEMU_RUNNING;
  }

  uint64_t timer_start = get_time();

  execute(n);

  uint64_t timer_end = get_time();
  g_timer += timer_end - timer_start;

  switch (nemu_state.state) {
    case NEMU_RUNNING: nemu_state.state = NEMU_STOP; break;

    case NEMU_END: case NEMU_ABORT:
      Log("nemu: %s at pc = " FMT_WORD,
          (nemu_state.state == NEMU_ABORT ? ANSI_FMT("ABORT", ANSI_FG_RED) :
           (nemu_state.halt_ret == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) :
            ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
          nemu_state.halt_pc);
      // fall through
    case NEMU_QUIT: statistic();
  }
}
