# C4 — Claude Code Instructions

## Project Identity
- **App**: Minimal, zero-copy HTTP/1.1 server framework for C++, inspired by Express
- **Language**: C++20, following the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html) — do not use C++23 features, do not use non-standard extensions
- **Build system**: CMake (>= 3.16) exclusively
- **Status**: early scaffold — HTTP framework itself is not implemented yet
- **Commands**: `cmake -B build -DCMAKE_BUILD_TYPE=Debug` · `cmake --build build` · `ctest --test-dir build --output-on-failure`
- **Note**: this project was originally C-only. C concepts learned in `docs/learn/` (kept as `.c`) are ported to idiomatic C++ in `include/`/`src/`. Future C bindings/support may be added later, but are not a current goal.

## Architecture Rules (NEVER violate these)
- Public API surface lives only in `include/c4/*.h` — implementation details never leak into headers
- Headers use the `.h` extension, implementation files use the `.cc` extension (Google convention)
- Public API is scoped inside the `c4` namespace, not exposed via free functions at global scope

## Coding Standards
- Style is enforced by `.clang-format` (Google) and `.clang-tidy`
- Follow Google C++ Style Guide naming: `PascalCase` for functions and types, `snake_case` for variables, trailing `_` for private members

## Testing Rules
- Every new public function in `include/c4/*.h` gets a corresponding test in `tests/`, registered in `tests/CMakeLists.txt`
- Tests use `assert()` plus an explicit non-zero return on failure (see `tests/test_hello.cc`) — don't rely on `assert()` alone since `NDEBUG` can strip it
- Tests must pass under AddressSanitizer/UndefinedBehaviorSanitizer (the Debug build default) — a test that only passes in Release is not done
- Run `ctest --test-dir build --output-on-failure` before considering any change complete

## File Structure
```
include/c4/       ← Public headers (.h) — the only files consumers include
src/               ← Implementation (.cc files), CMakeLists.txt registers each source
tests/             ← ctest-registered test executables (.cc), one assertion suite per unit
examples/          ← Standalone runnable programs demonstrating usage of the public API (empty until the framework exists)
docs/learn/        ← Reference-only scratch code from learning sessions in C (not built by CMake)
cmake/             ← Shared CMake modules (CompilerWarnings.cmake, etc.)
build/             ← Generated — never hand-edit, never commit
```

## Dependencies — APPROVED LIST
This project targets the C++ standard library and POSIX sockets only. Ask before adding anything new.
- Standard C++20 library (`<string>`, `<string_view>`, `<vector>`, `<memory>`, `<cassert>`, …)
- POSIX sockets API (`sys/socket.h`, `netinet/in.h`, `unistd.h`)
- No third-party C++ libraries, no vendored dependencies, no package manager (vcpkg/conan) without explicit approval

## Git Behavior
- Commits follow Conventional Commits: `feat(scope): summary`, `fix(scope): summary`, `chore(scope): summary`, `style(scope): summary`, `docs(scope): summary`
- Group files by purpose — one commit per semantic change, never a single mega-commit
- Do NOT push automatically — leave push for the user

## What to NEVER do
- Never commit anything under `build/`
- Never add a third-party dependency without asking first
