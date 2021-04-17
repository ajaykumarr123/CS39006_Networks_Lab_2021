#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define MAXLINE 10

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void dostuff (int sockfd,int new_socket)
{
    char buff[100];      
    bzero(buff,100);
    int n = read(new_socket,buff,99);    //recieving filename
    buff[n]='\0';   

    char* fname = buff;
        
    // filename is received, server will open the file and send the chunks
    // of data in strings, marking the end of a chunk by a '$'
    
    printf("CLIENT: %s\n", fname);
   
    int fd = open(fname, O_RDONLY , 0); 
    
    if(fd == -1)
    {   close(sockfd);
        close(new_socket);
        
        exit(0);
    }

    else
    {
        printf("[+]File open successful...\n");
        send(new_socket,"$",strlen("$"),0);
    }

    while(1)
    {
        char temp[MAXLINE];
        int len = read (fd, temp, MAXLINE-1); 
        if(len<=0)
            break;
        temp[len] = '\0';
        //printf("%s\n",temp);
        write(new_socket,(const char*)temp,strlen(temp));
    } 

      printf("[+]File send successful...\n");
}

int main()
{
     int sockfd, newsockfd;
     socklen_t clilen;
     struct sockaddr_in server_addr, cli_addr;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("[-]ERROR opening socket\n");

     printf("[+]Server socket created successfully.\n");

     bzero((char *) &server_addr, sizeof(server_addr));
     server_addr.sin_family = AF_INET;
     server_addr.sin_addr.s_addr = INADDR_ANY;
     server_addr.sin_port = htons(8080);

     if (bind(sockfd, (struct sockaddr *) &server_addr,sizeof(server_addr)) < 0) 
          {
               bzero((char *) &server_addr, sizeof(server_addr));
 
             error("[-]ERROR on binding");
            }

     printf("[+]Binding successfull.\n");

     if(listen(sockfd, 10) == 0)
          printf("[+]Listening....\n");
     else
        error("[-]Error in listening\n");

     clilen = sizeof(cli_addr);

     while (1) {
        printf("-----------------------------------------------------------------------------\n");
         newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
         if (newsockfd < 0) 
             error("ERROR on accept");
         printf("[+]request accepted....\n");

         dostuff(sockfd,newsockfd);
         close(newsockfd);
     } 
     close(sockfd);
     return 0; 
}
