#include "common.h"
#include <amdev.h>
#include "klib.h"

size_t serial_write(const void *buf, size_t offset, size_t len) {
  //return 0;
  uint32_t i = 0;
  const char *temp = buf;
  for (; i < len; ++i) 
	_putc(temp[i]);
  return len;
}

#define NAME(key) \
  [_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};

size_t events_read(void *buf, size_t offset, size_t len) {
	int key = read_key();
  if (key == _KEY_NONE) 
      snprintf(buf, len, "t %d\n", uptime());
//sprintf(buf, "t %d\n", uptime());
  else if (key & 0x8000) 
  {
      key &= 0x7fff;
      snprintf(buf, len, "kd %s\n", keyname[key]);
//sprintf(buf, "kd %s\n", keyname[key]);
  }
  else 
  {
      key &= 0x7fff;
	  snprintf(buf, len, "ku %s\n", keyname[key]);
// sprintf(buf, "ku %s\n", keyname[key]);
  }
  return strlen(buf);
}

static char dispinfo[128] __attribute__((used));

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  //Log("%d %d",offset,len);
  memcpy(buf, dispinfo + offset, len);
//memcpy(buf, dispinfo, len);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  offset /= sizeof(uint32_t);
  int row = offset / screen_width();
  int col = offset % screen_width();
  draw_rect((uint32_t *)buf, col, row, len/4, 1);
  return len;
}

void init_device() {
  Log("Initializing devices...");
  _ioe_init();

  // TODO: print the string to array `dispinfo` with the format
  // described in the Navy-apps convention
  sprintf(dispinfo, "WIDTH:%d\nHEIGHT:%d\n", screen_width(), screen_height());
}
