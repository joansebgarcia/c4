# h3

Minimal, zero-copy HTTP/1.1 server framework for C++, inspired by Express.

This project is a C++ port of [h3](https://github.com/h3js/h3), the
minimal H(TTP) framework for JavaScript. It borrows h3's API design and
philosophy — a small, composable core with an Express-like routing
surface — and reimplements a simplified subset of it in modern C++,
without any of the original JavaScript source.

```cpp
#include "h3/app.h"

int main() {
  h3::App app;

  app.Get("/", [](const h3::Request& req, h3::Response& res) {
    res.Send("Hello World");
  });

  app.Listen(3000, [] {
    std::cout << "Server is running on http://localhost:3000\n";
  });
}
```

Everything under `include/h3/` is meant to stay this small: `App`,
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
