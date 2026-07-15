#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

volatile sig_atomic_t running = 1;

void handle_sigint(int sig) {
  (void) sig;
  running = 0;
}

int main(void) {
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("socket");
    exit(1);
  }

  struct sockaddr_in address = {0};
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(8080);

  int b = bind(server_fd, (struct sockaddr*) &address, sizeof(address));
  if (b == -1) {
    perror("IP");
    exit(1);
  }

  int l = listen(server_fd, 5);
  if (l == -1) {
    perror("listen");
    exit(1);
  }

  // Esto es una alternativa al uso original de `signal()` ya que en macOS/BSD
  // por defecto, cuando registras un handler con signal(),
  // el sistema usa "semántica BSD": si una syscall como accept() es
  // interrumpida por una señal, el kernel la reinicia automáticamente en vez de
  // devolver -1 con EINTR.
  struct sigaction sa = {0};
  sa.sa_handler = handle_sigint;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGINT, &sa, NULL);

  while (running) {
    struct sockaddr_in client_addr = {0};
    socklen_t client_len = sizeof(client_addr);

    int c_fd = accept(server_fd, (struct sockaddr*) &client_addr, &client_len);
    if (c_fd == -1) {
      if (errno == EINTR) {
        break;
      }

      perror("accept");
      exit(1);
    }

    char buffer[4096] = {0};
    ssize_t bytes_read = read(c_fd, buffer, sizeof(buffer) - 1);

    if (bytes_read == -1) {
      perror("read");
      exit(1);
    }

    const char* body = "hello from c4\n";
    char response[4096];
    int response_len = snprintf(
        response,
        sizeof(response),
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: %zu\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%s",
        strlen(body),
        body
    );

    ssize_t bytes_written = write(c_fd, response, (size_t) response_len);

    if (bytes_written == -1) {
      perror("write");
      exit(1);
    }

    close(c_fd);
  }

  close(server_fd);
  printf("closing server...\n");
}
