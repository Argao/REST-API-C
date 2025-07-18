/*
 * Servidor “Hello, Socket” — etapa 1
 * Compilar: gcc -Wall -Wextra -std=c11 -O0 -g main.c -o server
 * Executar: ./server
 */

#include <stdio.h>              // printf(), perror()
#include <stdlib.h>             // exit()
#include <string.h>             // memset()
#include <unistd.h>             // close()
#include <errno.h>              // errno
#include <sys/socket.h>         // socket(), bind(), listen(), accept(), send()
#include <netinet/in.h>         // struct sockaddr_in
#include <arpa/inet.h>          // htons()

#define PORT 8080               // Porta em que vamos escutar

int main(void)
{
    /* 1. Criar o socket (IPv4 + TCP) */
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("[+] Socket criado: fd=%d\n", listen_fd);

    /* 2. Ligá‑lo a uma porta/IP (bind) */
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof addr);
    addr.sin_family = AF_INET;          // IPv4
    addr.sin_addr.s_addr = htonl(INADDR_ANY); // 0.0.0.0 (todas as interfaces)
    addr.sin_port = htons(PORT);        // Converte 8080 para “network byte order”

    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof addr) == -1) {
        perror("bind");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }
    printf("[+] Bind em 0.0.0.0:%d\n", PORT);

    /* 3. Colocar no modo servidor (listen) */
    if (listen(listen_fd, /* backlog = */ 10) == -1) {
        perror("listen");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }
    printf("[+] Aguardando conexão…\n");

    /* 4. Aceitar UM cliente (bloqueia até alguém conectar) */
    int client_fd = accept(listen_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("accept");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }
    printf("[+] Cliente conectado! fd=%d\n", client_fd);

    /* 5. Enviar resposta HTTP mínima */
    const char *response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 12\r\n"
        "Content-Type: text/plain\r\n"
        "\r\n"
        "Hello World\n";
    ssize_t nbytes = send(client_fd, response, strlen(response), 0);
    if (nbytes == -1) {
        perror("send");
    } else {
        printf("[+] %zd bytes enviados\n", nbytes);
    }

    /* 6. Fechar sockets */
    close(client_fd);
    close(listen_fd);
    puts("[+] Conexões encerradas — fim do programa");
    return 0;
}
