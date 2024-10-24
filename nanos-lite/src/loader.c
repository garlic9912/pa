#include <proc.h>
#include <elf.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);
extern size_t get_ramdisk_size();


// extern char *elf_loader_file;

static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf32_Ehdr ehdr; 
  // 读取文件
  ramdisk_read(&ehdr, 0, get_ramdisk_size());
  // 读取 Program Headers
  Elf32_Phdr phdr[ehdr.e_phnum];
  ramdisk_read(phdr, ehdr.e_phoff, ehdr.e_phnum * ehdr.e_phentsize);
  for (int i = 0; i < ehdr.e_phnum; ++i) {
    // LOAD Type
    if (phdr[i].p_type == PT_LOAD) {
      // printf("%x\n", phdr[i].p_paddr);
      printf("%x\n", 83000000);
      // ramdisk_write(&phdr[i].p_vaddr, phdr[i].p_vaddr-ramdisk_start, phdr[i].p_memsz);
      // memset(&phdr[i].p_vaddr+phdr[i].p_filesz, 0, phdr[i].p_memsz-phdr[i].p_filesz);
    }
  }  
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

