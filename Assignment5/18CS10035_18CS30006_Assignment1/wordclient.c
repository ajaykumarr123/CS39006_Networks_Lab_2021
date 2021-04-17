#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define MAXLENGTH 1024 
#define WORDLENGTH 40
#define FILE_NAME "inp.txt" 
#define RECIEVED_FILE_NAME "recieved.txt" 

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int main()
{	
	int sockfd,n;											
	socklen_t addr_len;
	char buff[MAXLENGTH];
	char REQ_WORD[WORDLENGTH];


	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sockfd < 0) 
        error("ERROR opening socket");

    struct sockaddr_in server_addr; 
    bzero(&server_addr, sizeof(server_addr));
    
    server_addr.sin_family = AF_INET; 
    server_addr.sin_port = htons(8080); 
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    

    // sending filename 
    sendto(sockfd, (const char *)FILE_NAME, strlen(FILE_NAME), MSG_CONFIRM, (const struct sockaddr *) &server_addr, sizeof(server_addr)); 
    n = recvfrom(sockfd, (char *)buff, MAXLENGTH, MSG_WAITALL, (struct sockaddr *) &server_addr, &addr_len); 
    buff[n] = '\0'; 
        
   

    if (strcmp(buff, "HELLO") == 0)
    {	
        FILE* fptr = fopen(RECIEVED_FILE_NAME, "w");
        int i = 1;

        char snum[20];
        while (1)
        {	            
    		
            bzero(REQ_WORD, sizeof(char));
            strcat(REQ_WORD, "WORD");
            int num=i,cur=0;
            while(num)
            {	snum[cur]='0'+num%10;
            	num/=10;
            	cur++;
            }

            for(int j=1;j<=cur;j++)
            {
            	REQ_WORD[j+3] = snum[cur-j];
            }
            
       

            sendto(sockfd, (const char *)REQ_WORD, strlen(REQ_WORD), MSG_CONFIRM, (const struct sockaddr *) &server_addr, sizeof(server_addr)); 
            bzero(buff,sizeof(buff));
            n = recvfrom(sockfd, (char *)buff, MAXLENGTH, MSG_WAITALL, (struct sockaddr *) &server_addr, &addr_len); 
            buff[n] ='\0';

            if (strcmp(buff, "END") == 0)
                break;
            else if (strcmp(buff, "WRONG_WORD_REQ_WORD_FORMAT") == 0)
            {
                printf("Wrong Word REQ_WORD Format\n");
                break;
            }
            
            else 
            {
                if (strlen(buff)>0)
                {
                    fputs(buff, fptr);
                    fputs("\n", fptr);
                }
            }
            i++;
        }

        fclose(fptr);
    }

    else if (strcmp(buff,"FILE_NOT_FOUND") == 0)
        error("FILE NOT FOUND"); 
    

    else if (strcmp(buff, "WRONG_FILE_FORMAT") == 0)
        error("Wrong File Format"); 
      

    close(sockfd); 
    return 0; 
} 