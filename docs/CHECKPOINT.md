# Checkpoint — día 2 (2026-07-15)

## Qué se construyó (en `docs/learn/`, código de referencia, sin build por CMake)

1. `tcp.c` — request/response de un solo tiro, el servidor muere después de atender un cliente.
2. `tcp_loop.c` — `while (running)` alrededor de accept/read/write/close, más cierre limpio con `sigaction` + `SIGINT` + `EINTR` (nota: `signal()` normal falla en macOS/BSD porque activa `SA_RESTART` por defecto y se traga el `EINTR` que el loop necesita).
3. `tcp_http.c` — respuesta HTTP/1.1 real: status line, `Content-Type`, `Content-Length` calculado dinámicamente con `snprintf`, `Connection: close`.
4. `tcp_http_pro.c` — mismo comportamiento, refactorizado en funciones (`create_listening_socket`, `handle_client`, `build_hello_response`, helper `die()`) como semillas para el framework real.

## Housekeeping del repo (hecho en sesión, pendiente de commit)

- `examples/` vaciado — `CMakeLists.txt` queda como placeholder, reservado para demos reales de uso del framework más adelante.
- `CLAUDE.md` actualizado con `docs/learn/` en `File Structure`.
- `.clang-format`: `BinPackParameters/BinPackArguments: false` + `AlignAfterOpenBracket: BlockIndent` (formato un-argumento-por-línea estilo Rust).
- `.claude/commands/teach.md` — modo instructor Socrático activable con `/teach`.

## Decisión de arquitectura (conversación, aún sin construir)

Nada de `libuv` como dependencia — violaría la regla de solo-POSIX/sin-terceros de `CLAUDE.md`, y le quitaría al usuario la parte que quiere aprender. Plan: event loop propio, primero con `select()` (simple, portable), después posiblemente `kqueue` (nativo de macOS, lo que usa `libuv` por debajo en BSD/macOS).

## Pendiente para la siguiente sesión (sin decidir orden)

- (a) Parsear la request HTTP real (método + ruta + headers) desde `buffer` hacia una struct.

