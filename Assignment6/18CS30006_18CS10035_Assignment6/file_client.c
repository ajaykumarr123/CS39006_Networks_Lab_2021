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

#define FILE_NAME_RECIEVED "test_recieved.txt" 
#define MAXLENGTH 100
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void dostuff(int sockfd)
{
	char buff[MAXLENGTH];
	int n;
	printf("Enter Filename: ");
	scanf("%s",buff);

	write(sockfd,buff,strlen(buff));	//sending filename
	 
  	int fd, set = 0;
  	int char_count=0;
  	int word_count=0;

  	memset(buff,0,strlen(buff));
	int flag = recv(sockfd,buff,MAXLENGTH,0);

	if(flag ==0)
	{
		close(sockfd);
		printf("ERR 01: File Not Found\n");
		exit(0);
	}

	else
	{
		fd = open(FILE_NAME_RECIEVED, O_WRONLY | O_CREAT | O_TRUNC, 0644);

		if(fd < 0)
		{
			close(sockfd);
			printf("ERROR while creating file\n");
			exit(1);
		}	
	}

	while(1)											//Recieve file content in chunks
	{	
		memset(buff,0,strlen(buff));
		int len = read(sockfd,buff,MAXLENGTH-1);
		// printf("%d \n",len );
		if(len>0)
		{
			buff[len]='\0';
			//printf("%s\n",buff);
			write(fd, buff, strlen(buff));
			char_count+=len;
			for(int i=0;i<len;i++)
			{	
				//printf("%d %c %d\n",i,buff[i],word_count);
				if(buff[i]==' '||buff[i]==';'||buff[i]=='.'||buff[i]==','||buff[i]=='\t' || buff[i]=='\n')
				{
					set=0;			
				}

				else if(set==0)
				{
					set=1;
					word_count++;
				}	
			}	
		}

		else
		{
			printf("The file transfer is successful. Size of the file = %d bytes, no. of words = %d\n",char_count,word_count);
			break;
		}

	}
}

int main()
{
	char fname[25];
	int len,sockfd;
	struct sockaddr_in server_addr,cliaddr;
	sockfd=socket(AF_INET,SOCK_STREAM,0);

	if(sockfd<0)
		error("[-]Error in socket");

	printf("[+]Server socket created successfully.\n");

	bzero(&server_addr,sizeof(server_addr));
	server_addr.sin_family=AF_INET;
	server_addr.sin_addr.s_addr=INADDR_ANY;
	server_addr.sin_port=htons(8080);

	if(connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0)
		error("[-]Error while connecting\n");

	printf("[+]Connected to Server.\n");

	dostuff(sockfd);
	close(sockfd);

	return 0;
}
