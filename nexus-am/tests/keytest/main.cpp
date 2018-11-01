#include <am.h>
#include <klib.h>
#include <amdev.h>

#define NAME(key) \
  [_KEY_##key] = #key,

const char *names[256] = {
  [_KEY_NONE] = "NONE",
  _KEYS(NAME)
};
int main() {
  _ioe_init();

  while (1) {
    int key = read_key();
    bool down = false;
    if (key & 0x8000) {
	  printf("key1=%d\n",key);
      key ^= 0x8000;
      down = true;
	  printf("key2=%d\n",key);
    }
    if (key != _KEY_NONE) {
      printf("Get key: %d %s %s\n", key, names[key], down ? "down" : "up");
    }
  }
  return 0;
}
