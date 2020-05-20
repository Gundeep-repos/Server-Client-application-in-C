//Indvidual Participation: Gundeep Singh (110008447)

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    char message[255];
    int server, portNumber, dice;
    long int ss = 0;
    socklen_t len;

    struct sockaddr_in servAdd;

    if (argc != 3)
    {

        printf("Call model: %s <IP> <PORT#>\n", argv[0]);
        exit(0);
    }

    if ((server = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "Cannot create a socket\n");
        exit(1);
    }

    servAdd.sin_family = AF_INET;
    sscanf(argv[2], "%d", &portNumber);
    servAdd.sin_port = htons((uint16_t)portNumber);

    if (inet_pton(AF_INET, argv[1], &servAdd.sin_addr) < 0)
    {
        fprintf(stderr, "inet_pton() has failed\n");
    }

    if (connect(server, (struct sockaddr *)&servAdd, sizeof(servAdd)) < 0)
    {
        fprintf(stderr, "connect() has failed, exiting\n");
        exit(3);
    }

    while (1)
    {

        if (read(server, message, 255) < 0)
        {
            fprintf(stderr, "read() error");
            exit(3);
        }

        fprintf(stderr, "Server's message: %s\n", message);

        if (!(strcmp(message, "You can now play")))
        {
            // printf("Its my chance to play now\n");
            dice = (int)(time(&ss)) % 10 + 1;
            printf("Got this on dice: %d\n", dice);
            sprintf(message, "%d", dice);
            printf("here is the string value: %s\n", message);
            write(server, message, strlen(message) + 1);
        }
        if (!(strcmp(message, "Game over: you won the game")))
        {
            printf("I won the game\n");
            close(server);
            exit(0);
        }
        if (!(strcmp(message, "Game over: you lost the game")))
        {
            printf("I lost the game\n");
            close(server);
            exit(0);
        }
    }
}
