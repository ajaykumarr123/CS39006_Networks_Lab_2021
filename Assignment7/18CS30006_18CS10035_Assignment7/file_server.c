#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#define PORT 50000 
#define MAXLENGTH 1024
#define B 20

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void dostuff (int sockfd,int new_socket_fd)
{
    struct stat st;
    int len;
    char buff[MAXLENGTH];      
    bzero(buff,MAXLENGTH);
    int n = read(new_socket_fd,buff,MAXLENGTH-1);    //recieving filename
    buff[n]='\0';   

    char* fname = buff;
        
    // filename is received, server will open the file and send the chunks
    // of data in strings, marking the end of a chunk by a '$'
    
    printf("CLIENT: %s\n", fname);
   
    int fd = open(fname, O_RDONLY , 0); 
    
    if(fd == -1)
    {  
        send(new_socket_fd,"E",strlen("E"),0);
        printf("[-]FILE NOT FOUND...\n")  ;
    }

    else
    {
        printf("[+]File open successful...\n");
        send(new_socket_fd,"L",strlen("L"),0);
    
        stat(fname, &st);            //find filesize
        int FSIZE = st.st_size;
      
       
        // Send filesize
        send(new_socket_fd,&FSIZE,sizeof(FSIZE),0); 

        int no_of_complete_blocks = FSIZE/B, last_block_size= FSIZE%B;
        int total_blocks=0;
        while((len = read(fd,buff,B))>0)
        {
            total_blocks++;
            send(new_socket_fd,buff,len,0 );
        }

        if(last_block_size==0 && FSIZE!=0)
            last_block_size=B;
        printf("[+]file send successful.......\n");
        printf("Total number of blocks send = %d, Last block size = %d\n",total_blocks, last_block_size);
        
    } 
    close(fd);
    
}

int main()
{
     int sockfd, new_socket_fd;
     socklen_t clilen;
     struct sockaddr_in server_addr, cli_addr;

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("[-]ERROR opening socket\n");

     printf("[+]Server socket created successfully.\n");

     bzero((char *) &server_addr, sizeof(server_addr));
     server_addr.sin_family = AF_INET;
     server_addr.sin_addr.s_addr = INADDR_ANY;
     server_addr.sin_port = htons(PORT);

     if (bind(sockfd, (struct sockaddr *) &server_addr,sizeof(server_addr)) < 0) 
           error("[-]ERROR on binding");
            
     printf("[+]Binding successfull.\n");

     if(listen(sockfd, 10) == 0)
          printf("[+]Listening....\n");
     else
        error("[-]Error in listening\n");

     clilen = sizeof(cli_addr);

     while (1) {
        printf("-----------------------------------------------------------------------------\n");
         new_socket_fd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
         if (new_socket_fd < 0) 
             error("ERROR on accept");
         printf("[+]request accepted....\n");

         dostuff(sockfd,new_socket_fd);
         close(new_socket_fd);
     } 
     close(sockfd);
     return 0; 
}
