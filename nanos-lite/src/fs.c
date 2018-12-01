#include <unistd.h>
#include "fs.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
int fs_open(const char *pathname, int flags, int mode);
ssize_t fs_read(int fd, void *buf, size_t len);
ssize_t fs_write(int fd, const void *buf, size_t len);
off_t fs_lseek(int fd, off_t offset, int whence);
int fs_close(int fd);
size_t fs_filesz(int fd);
extern size_t ramdisk_read();
extern size_t ramdisk_write();

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  off_t open_offset;
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
  {"stdin", 0, 0, 0, invalid_read, invalid_write},
  {"stdout", 0, 0, 0, invalid_read, invalid_write},
  {"stderr", 0, 0, 0, invalid_read, invalid_write},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

int fs_open(const char *pathname, int flags, int mode)
{
	for(int i = 0; i < NR_FILES; i++)
	{
		if(strcmp(file_table[i].name, pathname) == 0)
			return i;
	}
	assert(0);
	return -1;
}

ssize_t fs_read(int fd, void *buf, size_t len)
{
	ssize_t fs_size = fs_filesz(fd);
	ssize_t fs_offset = file_table[fd].open_offset;
	if(fs_offset >= fs_size)
		return 0;
    if(fs_offset + len > fs_size)
	    len = fs_size - fs_offset;
	ramdisk_read(buf, fs_offset + file_table[fd].disk_offset, len);
	file_table[fd].open_offset += len;
	return len;
}

ssize_t fs_write(int fd, const void *buf, size_t len)
{
	ssize_t fs_size = fs_filesz(fd);
	ssize_t fs_offset = file_table[fd].open_offset;
	if(fs_offset >= fs_size)
		return 0;
    if(fs_offset + len > fs_size)
	    len = fs_size - fs_offset;
	ramdisk_write(buf, fs_offset + file_table[fd].disk_offset, len);
	file_table[fd].open_offset += len;
	return len;	
}

int fs_close(int fd)
{
	return 0;
}

size_t fs_filesz(int fd) 
{
	 return file_table[fd].size;
}
