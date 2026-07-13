# c4 — Project Scaffold Design

Date: 2026-07-13

## Purpose

Set up a professional C17 project skeleton for **c4**, a minimal HTTP/1.1
framework inspired by Express.

## Non-goals

- No HTTP parsing, routing, sockets, or any framework API. That is future work.
- No dependency on third-party C libraries.

## Build system

- **CMake**, minimum version 3.16.
- `CMAKE_C_STANDARD 17`, `CMAKE_C_STANDARD_REQUIRED ON`.
- `libc4` built as a **static library** target `c4`, aliased as `c4::c4`.
- Public headers exposed via `target_include_directories` on `include/`.

## Layout

```
c4/
├── CMakeLists.txt
├── include/
│   └── c4/
│       └── c4.h           
├── src/
│   ├── CMakeLists.txt
│   └── c4.c                
├── examples/
│   ├── CMakeLists.txt
│   └── hello.c             
├── tests/
│   ├── CMakeLists.txt
│   └── test_hello.c        
├── cmake/
│   └── CompilerWarnings.cmake
├── .clang-format
├── .clang-tidy
├── .github/workflows/ci.yml
├── .gitignore                
├── LICENSE                    
└── README.md               
```

## Components

- **`include/c4/c4.h`**: single public header declaring `const char *c4_hello(void);`
  (or equivalent minimal API) — just enough surface area to prove the library
  boundary works. No other declarations.
- **`src/c4.c`**: implementation backing the header.
- **`examples/hello.c`**: a `main()` that links against `c4` and prints the
  hello-world output to stdout. This is the only executable behavior.
- **`tests/test_hello.c`**: a CTest test executable that calls the same public
  function and asserts on its return value via `assert.h`. Registered with
  `add_test()` in `tests/CMakeLists.txt`. No external test framework.

## Tooling

- **Warnings/sanitizers** (`cmake/CompilerWarnings.cmake`): `-Wall -Wextra
  -Wpedantic -Werror` for all configs; `-fsanitize=address,undefined` added to
  compile/link flags when `CMAKE_BUILD_TYPE=Debug`.
- **`.clang-format`**: base C style config (LLVM-derived, 4-space or similar —
  implementer's reasonable default) to keep formatting consistent from the
  first commit.
- **`.clang-tidy`**: baseline set of checks appropriate for a C17 codebase
  (e.g. `bugprone-*`, `clang-analyzer-*`, `readability-*`), non-blocking.
- **CI (`.github/workflows/ci.yml`)**: GitHub Actions, matrix over `{gcc,
  clang}` on `ubuntu-latest`. Steps: configure with CMake, build, run
  `ctest --test-dir build --output-on-failure`. Triggers on push and pull
  request.

## Error handling

Not applicable — the scaffold has no error paths beyond what the compiler and
CTest already surface (build failures, test assertion failures, CI failures).

## Testing

One CTest case (`test_hello`) verifying the hello-world function's output.
This exists purely to prove the `tests/` + CTest wiring works; it is not meant
to cover future framework functionality.

## Out of scope / future work

- HTTP/1.1 parsing, connection handling, routing API, middleware — all
  deliberately deferred to a future spec once this scaffold is in place.