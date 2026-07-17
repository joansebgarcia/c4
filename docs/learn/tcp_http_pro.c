#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define C4_MAX_HEADERS 16
#define C4_PORT 8080
#define C4_BACKLOG 5
#define C4_BUFFER_SIZE 4096

typedef struct {
  char name[64];
  char value[192];
} c4_header;

typedef struct {
  char method[8];
  char path[256];
  c4_header headers[C4_MAX_HEADERS];
  size_t header_count;
} c4_request;

static volatile sig_atomic_t g_running = 1;

static void handle_sigint(int sig) {
  (void) sig;
  g_running = 0;
}

static void die(const char* what) {
  perror(what);
  exit(1);
}

static int parse_request_line(const char* buffer, c4_request* req) {
  char line[C4_BUFFER_SIZE];
  snprintf(line, sizeof(line), "%s", buffer);

  char* method = strtok(line, " ");
  char* path = strtok(NULL, " ");

  if (method == NULL || path == NULL) {
    return -1;
  }

  snprintf(req->method, sizeof(req->method), "%s", method);
  snprintf(req->path, sizeof(req->path), "%s", path);

  return 0;
}

static int parse_headers(const char* buffer, c4_request* req) {
  const char* line_start = strstr(buffer, "\r\n");
  if (line_start == NULL) {
    return 1;
  }

  line_start += 2;  // Jump the "\r\n" of the request

  req->header_count = 0;

  while (line_start[0] != '\r' && req->header_count < C4_MAX_HEADERS) {
    const char* line_end = strstr(line_start, "\r\n");
    const char* colon = strchr(line_start, ':');
    const char* value_start = colon + 1;

    if (line_end == NULL) {
      return -1;
    }

    if (colon == NULL || colon > line_end) {
      return -1;
    }
    while (*value_start == ' ') {
      value_start++;
    }

    c4_header* header = &req->headers[req->header_count];
    snprintf(
        header->name,
        sizeof(header->name),
        "%.*s",
        (int) (colon - line_start),
        line_start
    );

    snprintf(
        header->value,
        sizeof(header->value),
        "%.*s",
        (int) (line_end - value_start),
        value_start
    );

    req->header_count++;
    line_start = line_end + 2;
  }

  return 0;
}

static int create_listening_socket(uint16_t port) {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    die("socket");
  }

  struct sockaddr_in address = {0};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  if (bind(server_fd, (struct sockaddr*) &address, sizeof(address)) == -1) {
    die("bind");
  }

  if (listen(server_fd, C4_BACKLOG) == -1) {
    die("listen");
  }

  return server_fd;
}

static void install_shutdown_handler(void) {
  struct sigaction sa = {0};
  sa.sa_handler = handle_sigint;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);
}

static size_t build_hello_response(char* out, size_t out_size) {
  const char* body = "hello from c4\n";

  int written = snprintf(
      out,
      out_size,
      "HTTP/1.1 200 OK\r\n"
      "Content-Type: text/plain\r\n"
      "Content-Length: %zu\r\n"
      "Connection: close\r\n"
      "\r\n"
      "%s",
      strlen(body),
      body
  );

  return (size_t) written;
}

static void handle_client(int client_fd) {
  char request[C4_BUFFER_SIZE] = {0};

  ssize_t bytes_read = read(client_fd, request, sizeof(request) - 1);
  if (bytes_read == -1) {
    perror("read");
    close(client_fd);
    return;
  }

  c4_request req;
  if (parse_request_line(request, &req) == -1) {
    fprintf(stderr, "malformed request line\n");
    close(client_fd);
    return;
  }

  printf("method=[%s] path=[%s]\n", req.method, req.path);

  if (parse_headers(request, &req) == -1) {
    fprintf(stderr, "malformed headers\n");
    close(client_fd);
    return;
  }

  for (size_t i = 0; i < req.header_count; i++) {
    printf("header: [%s] = [%s]\n", req.headers[i].name, req.headers[i].value);
  }

  char response[C4_BUFFER_SIZE];
  size_t response_len = build_hello_response(response, sizeof(response));

  if (write(client_fd, response, response_len) == -1) {
    perror("write");
  }

  close(client_fd);
}

static void run_accept_loop(int server_fd) {
  while (g_running) {
    struct sockaddr_in client_addr = {0};
    socklen_t client_len = sizeof(client_addr);

    int client_fd =
        accept(server_fd, (struct sockaddr*) &client_addr, &client_len);
    if (client_fd == -1) {
      if (errno == EINTR) {
        break;
      }
      die("accept");
    }

    handle_client(client_fd);
  }
}

int main(void) {
  int server_fd = create_listening_socket(C4_PORT);
  install_shutdown_handler();

  run_accept_loop(server_fd);

  close(server_fd);
  printf("closing server...\n");
}
