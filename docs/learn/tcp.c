#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(void) {
  /* 1. socket(): reserva un file descriptor TCP/IPv4 en el kernel.
   *    Todavia no tiene puerto ni direccion asociados. */
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("file descriptor assignment failed");
    exit(1);
  }

  /* 2. bind(): asocia ese socket a una IP + puerto de esta maquina,
   *    para que el kernel sepa a que fd entregar los paquetes que
   *    lleguen al puerto 8080. htons() convierte a network byte order. */
  struct sockaddr_in address = {0};

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(8080);

  int b = bind(server_fd, (struct sockaddr*) &address, sizeof(address));
  if (b == -1) {
    perror("IP association failed");
    exit(1);
  }

  /* 3. listen(): pone el socket en modo escucha; backlog=5 es la cola
   *    de conexiones establecidas que aun no se han recogido con accept(). */
  int l = listen(server_fd, 5);
  if (l == -1) {
    perror("listen failure");
    exit(1);
  }

  /* 4. accept(): bloquea hasta que llega un cliente. Devuelve un fd
   *    NUEVO y distinto (c_fd) que representa esa conexion puntual;
   *    server_fd sigue siendo solo el socket que escucha. */
  struct sockaddr_in client_address = {0};

  socklen_t client_len = sizeof(client_address);

  int c_fd = accept(server_fd, (struct sockaddr*) &client_address, &client_len);
  if (c_fd == -1) {
    perror("accept");
    exit(1);
  }

  /* 5. read(): lee los bytes crudos que mando el cliente (la peticion
   *    HTTP en texto plano) desde c_fd, nunca desde server_fd. */
  char buffer[4096] = {0};

  ssize_t bytes_read = read(c_fd, buffer, sizeof(buffer) - 1);
  if (bytes_read == -1) {
    perror("read");
    exit(1);
  }

  /* 6. write(): responde al cliente por el mismo c_fd. Esto NO es una
   *    respuesta HTTP valida todavia (falta status line y headers),
   *    por eso curl la interpreta como HTTP/0.9. */
  const char* response = "hello from c4\n";

  ssize_t bytes_written = write(c_fd, response, strlen(response));
  if (bytes_written == -1) {
    perror("write");
    exit(1);
  }

  /* 7. close(): libera ambos file descriptors. Sin loop alrededor de
   *    accept(), el programa atiende una sola conexion y termina. */
  close(c_fd);
  close(server_fd);
}
