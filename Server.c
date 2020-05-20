//Indvidual Participation: Gundeep Singh (110008447)

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdint.h>
#include<inttypes.h>
#include<unistd.h>

void servicePlayers (int sd1,int sd2);
int main(int argc, char * argv[]){

    int sd, player1,player2, portNumber;
    
    socklen_t len;
    struct sockaddr_in servAdd;

    if(argc != 2){
        printf("call model: %s <PORT #>\n", argv[0]);
        exit(0);
    }

    if ((sd=socket(AF_INET, SOCK_STREAM,0))<0){

        fprintf(stderr, "cannot create socket \n");
        exit(1);
    }

    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    sscanf( argv[1], "%d", &portNumber );
    servAdd.sin_port = htons((uint16_t)portNumber);
    bind(sd,(struct sockaddr*)&servAdd, sizeof(servAdd));
    listen(sd, 5);

    while(1){
        
        player1 = accept(sd, (struct sockaddr*)NULL, NULL);
        printf("Got first player\n");
        
        player2 = accept(sd, (struct sockaddr*)NULL, NULL);
        printf("Got second player\n");
        printf("lets start the game\n");
        if(!fork()){
            servicePlayers(player1, player2);

        }
        close(player1);
        close(player2);
    }


}

void servicePlayers (int sd1, int sd2){

    int numeric1 = 0;
    int total1=0;
    int numeric2 = 0;
    int total2=0;
    char message[255];

    while(1){

        strcpy(message,"You can now play");
        write(sd1, message, strlen(message)+1);

        if(!read(sd1, message, 255)){
            close(sd1);
            fprintf(stderr, "BYE, player 1 dead, wait for a new Player\n");
            exit(0);
        }

        fprintf(stderr, "TOTO dice value: %s\n", message);
        //converting the string into integer
        sscanf(message,"%d",&numeric1);
        // printf("and in numeric integer its this %d\n",numeric1);
        total1 = total1+numeric1;
        printf("TOTO's total score so far is: %d\n",total1);
	printf("----------------------------------\n");
        sleep(2);
        if(total1 >= 100 ){
 	    printf("Game Over : TOTO Won the game.\n ");
            strcpy(message,"Game over: you won the game");
            write(sd1, message, strlen(message)+1);

            strcpy(message,"Game over: you lost the game");
            write(sd2, message, strlen(message)+1);

            close(sd1);
            close(sd2);
            exit(0);
        }

        //for second player

        strcpy(message,"You can now play");
        write(sd2, message, strlen(message)+1);

        if(!read(sd2, message, 255)){
            close(sd2);
            fprintf(stderr, "BYE, player 2 dead, wait for a new player\n");
            exit(0);
        }

        fprintf(stderr, "TITI dice value: %s\n", message);
        sscanf(message,"%d",&numeric2);
        // printf("and in numeric integer its this %d\n",numeric2);
        total2 = total2+numeric2;
        printf("TITI's total score so far is: %d\n",total2);
        printf("----------------------------------\n");
	sleep(2);

        if(total2 >= 100 ){
 	    printf("Game Over : TITI Won the game.\n ");
	    strcpy(message,"Game over: you won the game");
            write(sd2, message, strlen(message)+1);

            strcpy(message,"Game over: you lost the game");
            write(sd1, message, strlen(message)+1);

            close(sd1);
            close(sd2);
            exit(0);

        }



    }

}











