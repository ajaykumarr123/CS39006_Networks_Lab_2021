#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define PORT 50000 
#define FILE_NAME_RECIEVED "test_recieved.txt" 
#define MAXLENGTH 1024
#define B 20

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void dostuff(int sockfd)
{
	char buff[MAXLENGTH],fname[MAXLENGTH];

	int FSIZE=0,len;
	printf("Enter Filename: ");
	scanf("%s",fname);

	write(sockfd,fname,strlen(fname));	//sending filename
	 
  	memset(buff,0,strlen(buff));
	int flag = recv(sockfd,buff,1,MSG_WAITALL);

	if(flag==1 && buff[0]=='E')
	{
		printf("ERR 01: File Not Found\n");
	}

	else
	{
		//recieve filesize
		len = recv(sockfd, &FSIZE, sizeof(int), MSG_WAITALL);

		int fd = open(FILE_NAME_RECIEVED, O_WRONLY | O_CREAT | O_TRUNC, 0644);

		if(fd < 0)
		{
			close(sockfd);
			close(fd);
			printf("ERROR while creating file\n");
			exit(1);
		}	

		int no_of_complete_blocks = FSIZE/B, last_block_size= FSIZE%B;
		int total_blocks=0;
		for (int i = 0; i < no_of_complete_blocks; ++i)										//Recieve file content in blocks
		{	
			memset(buff,0,strlen(buff));
			len = recv(sockfd,buff,B,MSG_WAITALL);
			buff[B]='\0';
			write(fd, buff, strlen(buff));

			total_blocks++;
			// printf("%d \n",len );
		}

		if(last_block_size!=0)
		{
			memset(buff,0,strlen(buff));
			len = recv(sockfd,buff,B,MSG_WAITALL);
			buff[last_block_size]='\0';
			write(fd, buff, strlen(buff));
			total_blocks++;
		}

		else if(FSIZE!=0)
			last_block_size=B;

		printf("The file transfer is successful!\n");
		printf("Total number of blocks received = %d, Last block size = %d\n",total_blocks, last_block_size);
		close(fd);
	}

	
}

int main()
{
	int sockfd;
	struct sockaddr_in server_addr,cliaddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);

	if(sockfd<0)
		error("[-]Error in socket");

	printf("[+]Server socket created successfully.\n");

	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=INADDR_ANY;
	server_addr.sin_port=htons(PORT);

	if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
		error("[-]Error while connecting\n");

	printf("[+]Connected to Server.\n");

	dostuff(sockfd);
	close(sockfd);

	return 0;
}
