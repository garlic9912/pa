#ifndef __FS_H__
#define __FS_H__

#include <common.h>

#ifndef SEEK_SET
enum {SEEK_SET, SEEK_CUR, SEEK_END};
#endif


// 文件系统
int fs_open(const char *pathname, int flags, int mode);
int fs_read(int fd, void *buf, size_t len);
int fs_close(int fd);
int fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);


// 虚拟文件系
size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t offset, size_t len);


#endif
