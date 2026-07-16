#include <cassert>
#include <cstring>

#include "c4/c4.h"

int main() {
  const char* result = c4::Hello();
  assert(std::strcmp(result, "Hello, World!") == 0);
  if (std::strcmp(result, "Hello, World!") != 0) {
    return 1;
  }
  return 0;
}
