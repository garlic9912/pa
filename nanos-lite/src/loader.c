#include <proc.h>
#include <elf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

// extern char *elf_loader_file;

static uintptr_t loader(PCB *pcb, const char *filename) {
  // Elf64_Ehdr 是 64 位系统的 ELF 头部结构体
  Elf64_Ehdr ehdr; 
  // 读取文件
  int fd = open("/home/garlic/ics2023/nanos-lite/build/nanos-lite-riscv32-nemu.elf", O_RDONLY);
  size_t n = read(fd, &ehdr, sizeof(ehdr));
  if (n != sizeof(ehdr)) {
    panic("Failed to read ELF header");
    close(fd);
  }
  // 读取 Program Headers
  Elf64_Phdr phdr[ehdr.e_phnum];
for (int i = 0; i < ehdr.e_phnum; ++i) {
    printf("Program %d:\n", i);
    printf("Type: %x\n", phdr[i].p_type);
    printf("Offset: %lx\n", phdr[i].p_offset);
    printf("Virtual Address: %lx\n", phdr[i].p_vaddr);
    printf("Physical Address: %lx\n", phdr[i].p_paddr);
    printf("File Size: %lx\n", phdr[i].p_filesz);
    printf("Memory Size: %lx\n", phdr[i].p_memsz);
    printf("Flags: %lx\n", phdr[i].p_flags);
    printf("Alignment: %lx\n", phdr[i].p_align);
}  
panic("okokokokokokokokookokokok");
  return 0;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

