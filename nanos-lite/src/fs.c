#include "fs.h"
#include "klib.h"

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

extern size_t ramdisk_read();
extern size_t ramdisk_write();
extern size_t serial_write();
extern size_t dispinfo_read();
extern size_t fb_write();
extern size_t events_read();

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
  {"stdout", 0, 0, 0, invalid_read, serial_write},
  {"stderr", 0, 0, 0, invalid_read, serial_write},
  {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
  {"/proc/dispinfo", 128, 0, 0, dispinfo_read, invalid_write},
  {"/dev/events", 0, 0, 0, events_read, invalid_write},
#include "files.h"

};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
  file_table[FD_FB].size = screen_width() * screen_height() * sizeof(uint32_t);
}

int fs_open(const char *pathname, int flags, int mode)
{
	for(int i = 0; i < NR_FILES; i++)
	{
		if(strcmp(file_table[i].name, pathname) == 0)
			return i;
	}
	//Log("%s",pathname);
	assert(0);
	return -1;
}

ssize_t fs_read(int fd, void *buf, size_t len)
{
	Log("nmsl");
	ssize_t fs_size = fs_filesz(fd);
	ssize_t fs_offset = file_table[fd].open_offset;	 
	if(fs_offset >= fs_size)
		return 0;
	if(fs_offset + len > fs_size)
	    len = fs_size - fs_offset;
Log("size:%d offset:%d len:%d",fs_size, fs_offset, len);
Log("%d",fd);
	if(file_table[fd].read)
	{
		file_table[fd].open_offset += len;
		return file_table[fd].read(buf, fs_offset, len);
	}
	else
	{
		ramdisk_read(buf, fs_offset + file_table[fd].disk_offset, len);
		file_table[fd].open_offset += len;
		return len;
	}
}

ssize_t fs_write(int fd, const void *buf, size_t len)
{	
	Log("nmsl");
	ssize_t fs_size = fs_filesz(fd);
	ssize_t fs_offset = file_table[fd].open_offset;	
	if(fs_offset > fs_size)
		return 0;
    if(fs_offset + len > fs_size)
		len = fs_size - fs_offset;		
Log("size:%d offset:%d len:%d",fs_size, fs_offset, len);
	if(file_table[fd].write)
	{
		file_table[fd].open_offset += len;
		return file_table[fd].write(buf, fs_offset, len);
	}	
	else 
	{		
		ramdisk_write(buf, fs_offset + file_table[fd].disk_offset, len);
		file_table[fd].open_offset += len;
		return len;
	}
}

off_t fs_lseek(int fd, off_t offset, int whence) 
{
  Log("nmsl");
	switch (whence) 
  {
    case SEEK_SET: file_table[fd].open_offset = offset; break;
    case SEEK_CUR: file_table[fd].open_offset += offset; break;
    case SEEK_END: file_table[fd].open_offset = file_table[fd].size + offset; break;
    default: return -1;
  }
  if (file_table[fd].size < file_table[fd].open_offset)
    file_table[fd].open_offset = file_table[fd].size;
  return file_table[fd].open_offset;
}


int fs_close(int fd)
{
	return 0;
}

size_t fs_filesz(int fd) 
{
	  return file_table[fd].size;
}
