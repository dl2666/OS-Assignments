#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#define BUFFSIZE 1024

/* functions for counting digits in string from client - returns number, and control C interrupt function which takes in ASCII value of 'C' and prints #lines read so far and total sum of digits counted*/
int lines = 0, digitSum = 0;

int countDigits(char *c){
    int x = 0;
    while(*c != '\0'){
        if(isdigit(*c))
            x++;
        c++;
    }
    return x;
}
void ctrlC(int x){
    printf("\nTotal number of lines received by the server: %d\n", lines);
    printf("\nTotal digit count across all input: %d\n", digitSum);
}

int main(int argc, char *argv[]){
//CtrlC interrupt handler
    signal(SIGINT, ctrlC);
    int servSock = 0, clientSock = 0, digits = 0, msgSize = 1;//, portNum;
    int sockaddr_len = sizeof(struct sockaddr_in);
    struct sockaddr_in serv_addr, clnt_addr;
    char buffer[BUFFSIZE], *test;
    FILE *filePtr;
    
    if (argc<2){
        printf("Error, no port provided");
        printf("Port: %s 5000\n",argv[0]);
        exit(1);
    }
    
    //file creation for secret code
    filePtr = fopen("secrets.out","w");
    fclose(filePtr);
    
    //initializing socket
    servSock = socket(AF_INET, SOCK_STREAM, 0);
    if (servSock < 0){
        perror("socket failed");
        exit(1);
    }
    //setting up memory block for server connection
    memset(&serv_addr, '0', sockaddr_len);
    
    //portNum = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bzero(&serv_addr.sin_zero,8);
    
    //bind socket to IP address/port - error checking if unable to bind()
    if((bind(servSock,(struct sockaddr*)&serv_addr,sockaddr_len))<0)
    {
        perror("Unable to bind\n");
        exit(1);
    }
    //server now waits for client to connect, backlog of size 1 connection
    listen(servSock, 1);
    

    while(1)
    {
        socklen_t addr_size = sizeof(clnt_addr);
        if((clientSock = accept(servSock, (struct sockaddr*)&clnt_addr, &addr_size))<0)
        {
            perror("accept() error");
            exit(1);
        }
        //Receive message from client
        if((msgSize = recv(clientSock,buffer, BUFFSIZE,0))<0)
        {
            perror("recv() failed");
            exit(1);
        }
        lines++;
        //last byte set to null
        buffer[msgSize]= '\0';
        printf("String received by server: %s \n", buffer);
        
        filePtr = fopen("secrets.out","a");
        fputs(buffer,filePtr);
        digits = countDigits(buffer);
        fprintf(filePtr,"Digits: %d \n",digits);
            
        digitSum+=digits;
        fclose(filePtr);
        
    
       close(clientSock);
     }
    
    return 0;
}

