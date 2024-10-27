#include <proc.h>
#include <elf.h>
#include <fs.h>

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
  // 打开文件
  // int fd = fs_open(filename, 0, 0);

  // 读取 ELF Headers
  // if (fs_read(fd, &ehdr, sizeof(Elf32_Ehdr)) != sizeof(Elf32_Ehdr)) {
  //   panic("read ehdr wrong");
  // }
  ramdisk_read(&ehdr, 0, 37396);

  // 读取 Program Headers
  Elf32_Phdr phdr[ehdr.e_phnum];
  // if (fs_read(fd, phdr, ehdr.e_phnum * ehdr.e_phentsize) != ehdr.e_phnum * ehdr.e_phentsize) {
  //   panic("read phar wrong");
  // }
  ramdisk_read(phdr, ehdr.e_phoff, ehdr.e_phnum * ehdr.e_phentsize);

  // load the program
  for (int i = 0; i < ehdr.e_phnum; ++i) {
    if (phdr[i].p_type == PT_LOAD) {
      ramdisk_read((void *)phdr[i].p_vaddr, phdr[i].p_offset, phdr[i].p_memsz);
      memset((void *)(phdr[i].p_vaddr+phdr[i].p_filesz), 0, phdr[i].p_memsz-phdr[i].p_filesz);
    }
  }  
  return ehdr.e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

