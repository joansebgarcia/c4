# c4 Project Scaffold Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Stand up a professional CMake + C17 project skeleton for c4 with a working hello-world library, example, test, and CI — no HTTP framework logic yet.

**Architecture:** A CMake project builds `libc4` (static library, public headers under `include/c4/`) exposing a single `c4_hello()` function. `examples/hello.c` consumes the library as a standalone binary. `tests/test_hello.c` is a CTest-registered executable asserting the same function's output. Shared compiler warnings/sanitizer flags live in an INTERFACE target (`cmake/CompilerWarnings.cmake`) linked by every buildable target.

**Tech Stack:** C17, CMake >= 3.16, CTest, GitHub Actions (gcc + clang matrix on ubuntu-latest), clang-format, clang-tidy.

## Global Constraints

- C standard: C17 (`CMAKE_C_STANDARD 17`, `CMAKE_C_STANDARD_REQUIRED ON`, `CMAKE_C_EXTENSIONS OFF`).
- CMake minimum version: 3.16.
- No third-party C dependencies.
- Warnings as errors: `-Wall -Wextra -Wpedantic -Werror` on every target.
- Debug builds add `-fsanitize=address,undefined -fno-omit-frame-pointer` (compile and link).
- `libc4` is a static library, target name `c4`, alias `c4::c4`.
- Function braces (`AfterFunction: true`) go on their own line; control-statement braces (`if`/`for`/`while`) stay attached to the statement.
- Commit messages follow Conventional Commits (`feat:`, `test:`, `ci:`, `docs:`, `chore:`).
- No HTTP/routing/socket logic in this scaffold — out of scope per spec.

---

### Task 1: CMake project skeleton + static library

**Files:**
- Create: `CMakeLists.txt`
- Create: `cmake/CompilerWarnings.cmake`
- Create: `include/c4/c4.h`
- Create: `src/CMakeLists.txt`
- Create: `src/c4.c`
- Create: `.clang-format`
- Create: `.clang-tidy`

**Interfaces:**
- Produces: `c4_hello(void) -> const char *` declared in `include/c4/c4.h`, implemented in `src/c4.c`, exported as CMake target `c4::c4` (INTERFACE include dir: `include/`).
- Produces: CMake INTERFACE target `c4_warnings` (from `cmake/CompilerWarnings.cmake`) — every other target links this `PRIVATE`.

- [ ] **Step 1: Create the compiler warnings module**

`cmake/CompilerWarnings.cmake`:

```cmake
add_library(c4_warnings INTERFACE)

target_compile_options(c4_warnings INTERFACE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(c4_warnings INTERFACE
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
    )
    target_link_options(c4_warnings INTERFACE
        -fsanitize=address,undefined
    )
endif()
```

- [ ] **Step 2: Create the root CMakeLists.txt**

`CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)
project(c4 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(CompilerWarnings)

option(C4_BUILD_EXAMPLES "Build c4 examples" ON)
option(C4_BUILD_TESTS "Build c4 tests" ON)

add_subdirectory(src)

if(C4_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(C4_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
```

- [ ] **Step 3: Create the public header**

`include/c4/c4.h`:

```c
#ifndef C4_H
#define C4_H

#ifdef __cplusplus
extern "C"
{
#endif

const char *c4_hello(void);

#ifdef __cplusplus
}
#endif

#endif /* C4_H */
```

- [ ] **Step 4: Create the library implementation**

`src/c4.c`:

```c
#include "c4/c4.h"

const char *c4_hello(void)
{
    return "Hello, World!";
}
```

- [ ] **Step 5: Create the src/ CMakeLists.txt**

`src/CMakeLists.txt`:

```cmake
add_library(c4 STATIC c4.c)
add_library(c4::c4 ALIAS c4)

target_include_directories(c4
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(c4 PRIVATE c4_warnings)
```

- [ ] **Step 6: Add `.clang-format`**

`.clang-format`:

```yaml
BasedOnStyle: LLVM
IndentWidth: 4
ColumnLimit: 100
PointerAlignment: Right
AllowShortFunctionsOnASingleLine: None
AllowShortIfStatementsOnASingleLine: false
BreakBeforeBraces: Custom
BraceWrapping:
  AfterFunction: true
  AfterControlStatement: false
  AfterStruct: true
  AfterEnum: true
  AfterUnion: true
  BeforeElse: false
```

- [ ] **Step 7: Add `.clang-tidy`**

`.clang-tidy`:

```yaml
Checks: >
  bugprone-*,
  clang-analyzer-*,
  readability-*,
  performance-*,
  -readability-magic-numbers,
  -readability-identifier-length
WarningsAsErrors: ''
HeaderFilterRegex: '.*'
```

- [ ] **Step 8: Configure and build the library (examples/tests don't exist yet, so disable them)**

Run: `cmake -B build -DCMAKE_BUILD_TYPE=Debug -DC4_BUILD_EXAMPLES=OFF -DC4_BUILD_TESTS=OFF`
Expected: configure succeeds, no errors.

Run: `cmake --build build`
Expected: `libc4.a` builds successfully with no warnings.

- [ ] **Step 9: Commit**

```bash
git add CMakeLists.txt cmake/CompilerWarnings.cmake include/c4/c4.h src/CMakeLists.txt src/c4.c .clang-format .clang-tidy
git commit -m "feat: add c4 CMake project skeleton with libc4 hello-world function"
```

---

### Task 2: Example executable

**Files:**
- Create: `examples/CMakeLists.txt`
- Create: `examples/hello.c`

**Interfaces:**
- Consumes: `c4_hello(void) -> const char *` from `c4::c4` (Task 1).
- Produces: executable target `c4_hello` at `build/examples/c4_hello`.

- [ ] **Step 1: Create the example source**

`examples/hello.c`:

```c
#include <stdio.h>

#include "c4/c4.h"

int main(void)
{
    printf("%s\n", c4_hello());
    return 0;
}
```

- [ ] **Step 2: Create `examples/CMakeLists.txt`**

```cmake
add_executable(c4_hello hello.c)
target_link_libraries(c4_hello PRIVATE c4::c4 c4_warnings)
```

- [ ] **Step 3: Configure and build with examples enabled**

Run: `cmake -B build -DCMAKE_BUILD_TYPE=Debug -DC4_BUILD_TESTS=OFF`
Expected: configure succeeds.

Run: `cmake --build build`
Expected: `c4_hello` binary builds with no warnings.

- [ ] **Step 4: Run the example**

Run: `./build/examples/c4_hello`
Expected output: `Hello, World!`

- [ ] **Step 5: Commit**

```bash
git add examples/CMakeLists.txt examples/hello.c
git commit -m "feat: add c4_hello example executable"
```

---

### Task 3: CTest test

**Files:**
- Create: `tests/CMakeLists.txt`
- Create: `tests/test_hello.c`

**Interfaces:**
- Consumes: `c4_hello(void) -> const char *` from `c4::c4` (Task 1).
- Produces: CTest test named `test_hello`.

- [ ] **Step 1: Create the test source**

`tests/test_hello.c`:

```c
#include <assert.h>
#include <string.h>

#include "c4/c4.h"

int main(void)
{
    assert(strcmp(c4_hello(), "Hello, World!") == 0);
    return 0;
}
```

- [ ] **Step 2: Create `tests/CMakeLists.txt`**

```cmake
add_executable(test_hello test_hello.c)
target_link_libraries(test_hello PRIVATE c4::c4 c4_warnings)

add_test(NAME test_hello COMMAND test_hello)
```

- [ ] **Step 3: Configure and build with tests enabled**

Run: `cmake -B build -DCMAKE_BUILD_TYPE=Debug`
Expected: configure succeeds (examples + tests both on by default).

Run: `cmake --build build`
Expected: `test_hello` binary builds with no warnings.

- [ ] **Step 4: Run the test suite**

Run: `ctest --test-dir build --output-on-failure`
Expected: `100% tests passed, 0 tests failed out of 1`.

- [ ] **Step 5: Commit**

```bash
git add tests/CMakeLists.txt tests/test_hello.c
git commit -m "test: add CTest coverage for c4_hello"
```

---

### Task 4: CI workflow

**Files:**
- Create: `.github/workflows/ci.yml`

**Interfaces:**
- Consumes: root `CMakeLists.txt` build/test flow from Tasks 1-3 (no new C interfaces).

- [ ] **Step 1: Create the workflow file**

`.github/workflows/ci.yml`:

```yaml
name: CI

on:
  push:
  pull_request:

jobs:
  build-and-test:
    runs-on: ubuntu-latest
    strategy:
      matrix:
        compiler: [gcc, clang]
    steps:
      - uses: actions/checkout@v4

      - name: Configure
        run: cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=${{ matrix.compiler }}

      - name: Build
        run: cmake --build build

      - name: Test
        run: ctest --test-dir build --output-on-failure
```

- [ ] **Step 2: Validate the workflow locally by replaying its steps for both compilers**

Run: `rm -rf build && cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=gcc && cmake --build build && ctest --test-dir build --output-on-failure`
Expected: build succeeds, `100% tests passed, 0 tests failed out of 1`.

Run: `rm -rf build && cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang && cmake --build build && ctest --test-dir build --output-on-failure`
Expected: build succeeds, `100% tests passed, 0 tests failed out of 1`.

(If `clang` is not installed locally, note this in the task result — CI will still cover it on ubuntu-latest.)

- [ ] **Step 3: Commit**

```bash
git add .github/workflows/ci.yml
git commit -m "ci: add CI workflow building and testing c4 with gcc and clang"
```

---

### Task 5: README build instructions

**Files:**
- Modify: `README.md`

**Interfaces:**
- None (documentation only).

- [ ] **Step 1: Update README.md with build/run/test instructions**

Replace the full contents of `README.md` with:

```markdown
# c4

Minimal, zero-copy HTTP/1.1 server framework for C, inspired by Express.

> Status: early scaffold — the HTTP framework itself has not been implemented
> yet. This currently builds a hello-world library, example, and test suite
> to validate the toolchain.

## Requirements

- CMake >= 3.16
- A C17 compiler (gcc or clang)

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
```

- [ ] **Step 2: Verify the instructions work end to end**

Run: `rm -rf build && cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build && ./build/examples/c4_hello && ctest --test-dir build --output-on-failure`
Expected: prints `Hello, World!`, then `100% tests passed, 0 tests failed out of 1`.

- [ ] **Step 3: Commit**

```bash
git add README.md
git commit -m "docs: document build, run, and test instructions"
```
