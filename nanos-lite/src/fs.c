#include <fs.h>

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

// disk
extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);


// Finfo structure
typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;  // 每个文件的指针偏移量
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, 0, invalid_read, invalid_write},
#include "files.h"
};


int fs_open(const char *pathname, int flags, int mode) {
  // 获取文件 fd,也就是文件表的对应下标
  int fd = -1;
  for (int i = 0; i < sizeof(file_table)/sizeof(Finfo); i++) {
    if (strcmp(pathname, file_table[i].name) == 0) {
      fd = i;
      return fd;
    }
  }
  // 没有找到文件，直接报错
  // panic("没有对应的文件: %s", pathname);
  return -1;
}


size_t fs_read(int fd, void *buf, size_t len) {
  size_t fsize, disk_offset, open_offset;
  fsize = file_table[fd].size;
  disk_offset = file_table[fd].disk_offset;
  open_offset = file_table[fd].open_offset;
  // 计算文件的读写指针偏移量
  size_t start_offset = disk_offset + open_offset;
  // 越界判断
  if (start_offset >= disk_offset + fsize) {
    // panic("文件读写指针越界");
    return -1;
  }
  // 读写字节是否缩短来防止越界
  if (start_offset + len >= disk_offset + fsize) {
    len = disk_offset + fsize - 1 - start_offset;
  }
  ramdisk_read(buf, start_offset, len);
  // 更新读写指针
  file_table[fd].open_offset = open_offset + len;
  return len;
}


size_t fs_write(int fd, const void *buf, size_t len) {
  // 处理stdout和stderr
  if (fd == FD_STDOUT || fd == FD_STDERR) {
    // 输出到串口, 即设置对应的写函数
    file_table[fd].write = serial_write;
  } else {
    // 设置普通文件的写函数
    file_table[fd].write = ramdisk_write;
  }
  size_t fsize, disk_offset, open_offset;
  fsize = file_table[fd].size;
  disk_offset = file_table[fd].disk_offset;
  open_offset = file_table[fd].open_offset;  
  // 计算文件的读写指针偏移量
  size_t start_offset = disk_offset + open_offset;
  // 越界判断
  if (start_offset >= disk_offset + fsize) {
    // panic("文件读写指针越界");
    return -1;
  }
  // 读写字节是否缩短来防止越界
  if (start_offset + len >= disk_offset + fsize) {
    len = disk_offset + fsize - 1 - start_offset;
  }  
  // 执行
  panic("111111111111111111");
  file_table[fd].write(buf, start_offset, len);
  // 更新读写指针
  file_table[fd].open_offset = open_offset + len;  
  return len;
}



size_t fs_lseek(int fd, size_t offset, int whence){
  // SEEK_SET: 参数offset 即为新的读写位置
  if (whence == SEEK_SET) {
    file_table[fd].open_offset = offset;
  }
  // SEEK_CUR: 以目前的读写位置往后增加offset 个位移量
  else if (whence == SEEK_CUR) {
    file_table[fd].open_offset += offset;
  }
  // SEEK_END: 则将文件的偏移量设置为，文件长度加offset
  else if (whence == SEEK_END) {
    file_table[fd].open_offset = file_table[fd].size + offset;
  }
  return file_table[fd].open_offset;
}




int fs_close(int fd) {
  // if (fd <= 2 || fd > sizeof(file_table)/sizeof(Finfo)) {
  //   panic("fd:%d is wrong", fd);
  // }
  // file_table[fd].open_offset = 0;
  return 0;
}








void init_fs() {
  // TODO: initialize the size of /dev/fb
}
