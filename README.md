# c4

Minimal, zero-copy HTTP/1.1 server framework for C++, inspired by Express.

> Status: early scaffold — the HTTP framework itself has not been implemented
> yet. This currently builds a hello-world library and test suite to
> validate the toolchain; `samples/` is reserved for usage demos once the
> public API exists.

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
