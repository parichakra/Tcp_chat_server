#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#define MAXLINE 1024
typedef struct sockaddr SA;

void *receive_messages(void *arg) {
    int sockfd = *(int *)arg;
    char recvline[MAXLINE + 1];
    int n;

    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = '\0'; // Null-terminate the string
        printf("Message from other client: %s\n", recvline);
    }

    return NULL;
}

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    char text[MAXLINE];
    pthread_t tid;

    if (argc != 3) {
        printf("usage: a.out <IPaddress> <port>\n");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
        printf("inet_pton error for %s\n", argv[1]);
        exit(1);
    }

    if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
        printf("connect error\n");
        exit(1);
    }

    printf("Connected to the server. Type your messages below:\n");

    // Create a thread to receive messages
    pthread_create(&tid, NULL, receive_messages, (void *)&sockfd);

    while (fgets(text, MAXLINE, stdin) != NULL) {
        write(sockfd, text, strlen(text));
    }

    close(sockfd);
    return 0;
}
