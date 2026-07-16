# c4

Minimal, zero-copy HTTP/1.1 server framework for C++, inspired by Express.

> Status: early scaffold — the HTTP framework itself has not been implemented
> yet. This currently builds a hello-world library, example, and test suite
> to validate the toolchain.

## Requirements

- CMake >= 3.16
- A C++20 compiler (gcc or clang)

## Build

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Run the example

```sh
./build/examples/c4_hello
```

## Run the tests

```sh
ctest --test-dir build --output-on-failure
```
