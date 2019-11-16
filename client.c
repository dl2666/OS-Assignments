#include    <stdio.h>
#include    <stdlib.h>
#include    <unistd.h>
#include    <errno.h>
#include     <string.h>
#include    <sys/types.h>
#include    <sys/stat.h>
#include    <sys/socket.h>
#include    <netinet/in.h>
#include    <arpa/inet.h>
#include    <fcntl.h>
#define BUFFSIZE 1024

int main(int argc, char *argv[])
{
    int sockfd = 0,portNum;
    char *buffer=malloc(sizeof(char)*BUFFSIZE), *test;
    struct sockaddr_in serv_addr;
 

    if(argc < 3)
    {
        printf("\nError in args, enter correct IP address/Port number: %s 127.0.0.1 5000\n",argv[0]);
        exit(1);
    }
    portNum=atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(portNum);
    bzero(&serv_addr.sin_zero,8);


    if((connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr))) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    }

    while(1)
    {
        printf("Please enter any alpha-numeric string: ");
        memset(buffer,0,strlen(buffer));
        //reads in user input into buffer
        fgets(buffer,BUFFSIZE,stdin);
        
        test = strstr(buffer,"C00L");
        //send message if contains "C00L"
        if(test!=NULL){
            if(send(sockfd,buffer,strlen(buffer),0)<0)
                perror("send error");
        }
    
    }
    close(sockfd);
    
    return 0;
}
