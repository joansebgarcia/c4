# Checkpoint — día 3 (2026-07-16)

## Qué se construyó (en `docs/learn/tcp_http_pro.c`, código de referencia, sin build por CMake)

1. `c4_request` — struct con `method`, `path`, y un array fijo `headers[C4_MAX_HEADERS]` (`c4_header { name, value }`) + `header_count`.
2. `parse_request_line(buffer, req)` — separa `MÉTODO SP PATH SP VERSIÓN` con `strtok` sobre una copia mutable del buffer (`strtok` escribe `'\0'` in-place, por eso no puede operar sobre el `const char* buffer` original). Devuelve `-1` si falta algún token.
3. `parse_headers(buffer, req)` — parsea las líneas `Nombre: valor\r\n` hasta la línea en blanco que separa headers de body. Usa `strstr`/`strchr` + aritmética de punteros en vez de `strtok`, porque `strtok` colapsa delimitadores consecutivos y se comería la línea en blanco (marca de fin de headers). Copia los substrings exactos con `snprintf(..., "%.*s", (int)(fin - inicio), inicio)`, sin mutar el buffer original.
4. `handle_client` ahora llama a ambos parsers sobre el `request` crudo leído del socket, corta la conexión con un log a `stderr` si alguno falla, e imprime method/path/headers parseados (`printf` temporal, todavía no hay routing real — la respuesta sigue siendo siempre `build_hello_response`).

## Housekeeping del repo (pendiente de commit)

- Sesión corrida con `/teach` — preferencia de estilo detectada y guardada en memoria: para APIs de C desconocidas, dar el código directo + explicación del porqué, en vez de preguntas guiadas tipo Socrático (el usuario no es experto en C).

## Decisión de arquitectura (conversación, aún sin construir)

Antes de portar a C++ (`include/c4/`), cerrar el ciclo completo request→handler→response en C con un router mínimo — así el port a C++ no mezcla diseño de arquitectura con aprendizaje simultáneo de `string_view`/RAII/templates.

## Pendiente para la siguiente sesión (sin decidir orden)

- (a) ~~Parsear la request HTTP real (método + ruta + headers) desde `buffer` hacia una struct.~~ — hecho.
- (b) Router mínimo en C (`docs/learn/`): tabla método+path → función handler, para que la respuesta dependa de `req.method`/`req.path` en vez de ser siempre `build_hello_response`.
- (c) Recién después de (b): arrancar el port a C++ real en `include/c4/`.
