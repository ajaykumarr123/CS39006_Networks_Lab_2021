#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

#define MAXLENGTH 1024
#define WORDLENGTH 40

int main()
{
	int sockfd, n;
	socklen_t addr_len;
	struct sockaddr_in server_addr, client_addr;

	// Making Socket File Descriptor
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
	{
		perror("Opening socket");
		exit(0);
	}
	bzero(&server_addr, sizeof(server_addr));
	bzero(&client_addr, sizeof(client_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(8080);

	// Binding the socket with server address
	if(bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("binding");
		exit(0);
	}
	addr_len = sizeof(struct sockaddr_in);

	while(1)
	{
		// Getting file name
		char *buf = (char *)malloc(MAXLENGTH*sizeof(char));
		n = recvfrom(sockfd, buf, MAXLENGTH, MSG_WAITALL, (struct sockaddr *)&client_addr, &addr_len);
		buf[n] = '\0';
		FILE* fptr = fopen(buf, "r");

		if(fptr == NULL)
		{
			sendto(sockfd, "FILE_NOT_FOUND", strlen("FILE_NOT_FOUND"), MSG_CONFIRM, (const struct sockaddr *)&client_addr, addr_len);
			exit(0);
		}

		// WORDi (given in assignment) is stored in Word 
		char Word[WORDLENGTH];
		fscanf(fptr, "%s", Word);

		// Checking for HELLO as First Word
		if(strcmp(Word, "HELLO") == 0)
		{
			sendto(sockfd, "HELLO", strlen("HELLO"), MSG_CONFIRM, (const struct sockaddr *)&client_addr, addr_len);

			// Going through all the other WORDS
			for(int i=1; fscanf(fptr, "%s", Word) != EOF; i++)
			{
				bzero(buf, sizeof(char));
				n = recvfrom(sockfd, buf, MAXLENGTH, MSG_WAITALL, (struct sockaddr *)&client_addr, &addr_len);
				buf[n] = '\0';

				// WORD REQUEST FORMAT checking
				int errorflag = 0;
				if(buf[0] == 'W' && buf[1] == 'O' && buf[2] == 'R' && buf[3] == 'D')
				{
					int num = i;
					for(int j=n-1; j>3; j--)
					{
						if((num%10 + '0') != buf[j] || num == 0)
							errorflag = 1;
						num = num/10;
					}
				}
				else
					errorflag = 1;

				if(errorflag == 0)
					// Sending WORD to client
					sendto(sockfd, Word, strlen(Word), MSG_CONFIRM, (const struct sockaddr *)&client_addr, addr_len);
				else
				{
					sendto(sockfd, "WRONG_WORD_REQUEST_FORMAT", strlen("WRONG_WORD_REQUEST_FORMAT"), MSG_CONFIRM, (const struct sockaddr *)&client_addr, addr_len);
					break;
				}
			}
			fclose(fptr);
		}
		else
		{
			sendto(sockfd, "WRONG_FILE_FORMAT", strlen("WRONG_FILE_FORMAT"), MSG_CONFIRM, (const struct sockaddr *)&client_addr, addr_len);
			exit(0);
		}
	}

}