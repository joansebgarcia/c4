#include <cassert>
#include <string>

#include "c4/c4.h"

int main() {
  const std::string result = c4::Hello();
  assert(result == "Hello, World!");
  if (result != "Hello, World!") {
    return 1;
  }
  return 0;
}
