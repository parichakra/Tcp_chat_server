#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#define MAXLINE 1024
#define LISTENQ 10
typedef struct sockaddr SA;

int clients[2]; // Array to store client socket descriptors
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *client_handler(void *arg) {
    int connfd = *(int *)arg;
    char buff[MAXLINE];
    int n;

    while ((n = read(connfd, buff, MAXLINE)) > 0) {
        buff[n] = '\0'; // Null-terminate the string

        // Relay the message to the other client
        pthread_mutex_lock(&lock);
        for (int i = 0; i < 2; i++) {
            if (clients[i] != connfd && clients[i] != 0) { // Send to the other client
                write(clients[i], buff, strlen(buff));
            }
        }
        pthread_mutex_unlock(&lock);
    }

    // Remove client from list after disconnection
    pthread_mutex_lock(&lock);
    for (int i = 0; i < 2; i++) {
        if (clients[i] == connfd) {
            clients[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&lock);

    close(connfd);
    return NULL;
}

int main(int argc, char *argv[]) {
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    int port;
    socklen_t len;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    port = atoi(argv[1]);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);

    bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
    printf("Server is waiting for connections at port %d\n", port);

    listen(listenfd, LISTENQ);

    int client_count = 0;

    while (client_count < 2) {
        len = sizeof(cliaddr);
        connfd = accept(listenfd, (SA *)&cliaddr, &len);
        printf("Connection from %s, port %d\n",
               inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, (char[INET_ADDRSTRLEN]){}, INET_ADDRSTRLEN),
               ntohs(cliaddr.sin_port));

        pthread_mutex_lock(&lock);
        clients[client_count++] = connfd; // Add client to the list
        pthread_mutex_unlock(&lock);

        pthread_t tid;
        pthread_create(&tid, NULL, client_handler, (void *)&connfd);
        pthread_detach(tid); // Detach thread to prevent resource leaks
    }

    // Server can now handle two clients and relay messages
    printf("Two clients connected. Communication enabled.\n");

    // Keep the server running indefinitely
    while (1) {
        sleep(1);
    }

    close(listenfd);
    return 0;
}
