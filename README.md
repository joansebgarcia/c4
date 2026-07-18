# c4

Minimal, zero-copy HTTP/1.1 server framework for C++, inspired by Express.

> Status: early scaffold — the HTTP framework itself has not been implemented
> yet. This currently builds a hello-world library and test suite to
> validate the toolchain; `samples/` is reserved for usage demos once the
> public API exists.

```cpp
#include "c4/app.h"

int main() {
  c4::App app;

  app.Get("/", [](const c4::Request& req, c4::Response& res) {
    res.Send("Hello World");
  });

  app.Listen(3000, [] {
    std::cout << "Server is running on http://localhost:3000\n";
  });
}
```

Everything under `include/c4/` is meant to stay this small: `App`,
`Request`, `Response`. Sockets, HTTP parsing, and routing are
implementation details that live in `src/` and never leak into a public
header.

## Requirements

- CMake >= 3.16
- A C++20 compiler (gcc or clang)

## Build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Run the tests

```sh
ctest --test-dir build --output-on-failure
```
