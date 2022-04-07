#include <bits/stdc++.h>
using namespace std;

#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h> 
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <sys/stat.h> 
#include <fcntl.h>
#include <sys/time.h> 
#include <errno.h> 
#include <sys/select.h> 


#define BUFFER_SIZE 1024
#define TIMEOUT 30	// Timeout value 30 sec

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

struct peer_info{
   string p_ip;
   int p_port;          // client port number
   int p_socket;		// socket used to communicate with a particular peer
   long p_last_inter;	// last interaction time with this client
   
   peer_info(string ip, int port) {
      
      p_ip = ip;
      p_port = port;
      p_socket = -1;
      p_last_inter = -1;   
  }
};


pair <string, string> parseinput(char * input_str) {
   int length = strlen(input_str);
   int f = 0;

   string person,message;
   for (int i = 0; i <length; i++) 
   {
      if (f == 0) 
      {
         if (input_str[i] == '/') 
         {
            f = 1;
            continue;
         }
         person.push_back(input_str[i]);
      } else message.push_back(input_str[i]);
   }
   return make_pair(person, message);
}

int main(int argc, char * * argv) {

	if (argc != 2) {
	  fprintf(stderr, "usage: %s <port>\n", argv[0]);
	  exit(1);
	}
	int portno = atoi(argv[1]);

    map <string, peer_info> mapp;
   
    ifstream myfile;
    myfile.open("users_info.txt");
   	
   	int no_of_peers,peer_port,user_exist_flag=0;
   	

    string peer_name, peer_ip;
    while (myfile >> peer_name) {
        myfile >> peer_ip >> peer_port;
        
        if(portno !=  peer_port) {
        	peer_info peer(peer_ip, peer_port);
        	mapp.insert(make_pair(peer_name,peer));
        	no_of_peers++;
    	}
    	else
    	{
    		cout<<"\t\t\t\tWelcome "<<peer_name<<"!"<<endl<<endl;
    		user_exist_flag=1;
    	}
    }
    myfile.close();

    if (user_exist_flag == 0) {
		printf ("You are not a registered User!. Please register your name at 'ip=127.0.0.1' with portno in users.txt file.\n");
		exit(1);
	}

    printf("-------------------------------Friends List----------------------------\n" );
    for (pair < string, peer_info> d : mapp) {
        cout << d.first<<" (" << d.second.p_ip << " : " << d.second.p_port << " )" << endl; 
    }
    printf("-----------------------------------------------------------------------\n" );
    printf("Chat Application Running ....\nmessage format: friendname/<msg>\n");
    printf("-----------------------------------------------------------------------\n" );


    struct sockaddr_in serveraddr,peer_addr;

    int srvsock = socket (AF_INET, SOCK_STREAM, 0);
	if (srvsock <0) error("ERROR opening socket");

	int optval = 1;
	setsockopt (srvsock, SOL_SOCKET, SO_REUSEPORT, (const void *) &optval, sizeof (int));
	
	// build the server's Internet address
	bzero((char *) &serveraddr, sizeof (serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl (INADDR_ANY);
	serveraddr.sin_port = htons ((unsigned short) portno); 		//  port to listen 

	if (bind (srvsock, (struct sockaddr *) &serveraddr, sizeof (struct sockaddr_in)) <0)
		error ("ERROR on binding");
 
	if (listen(srvsock, 5) <0) 
		error ("ERROR on listen");


	fd_set readset;
	struct timeval tv;

	while(1)
	{	
		FD_ZERO (&readset);
		FD_SET (srvsock, &readset);
		FD_SET (0, &readset);
		int maxfd = srvsock;

		for (auto d = mapp.begin(); d != mapp.end(); d++)
		{	
			if ((d->second).p_socket!=-1 ) 
			{	
				if((time(NULL)-(d->second).p_last_inter)>TIMEOUT){
					cout<<"[-]"<<d->first<<" timed-out. Closing connection ...\n";
					cout << "------------------------------------------" <<  endl;
					close((d->second).p_socket);
					(d->second).p_socket = -1;
				}
				else
				{
					FD_SET((d->second).p_socket, &readset);
					maxfd = (maxfd < (d->second).p_socket) ? (d->second).p_socket : maxfd;
				}
			}
		}
		

		tv.tv_sec =1;
		tv.tv_usec = 0;

	  	int flag = select (maxfd + 1, &readset, NULL, NULL,&tv);

	  	if (flag < 0 && errno != EINTR) {
	  		cout << "Error in select (): " << strerror (errno) << endl;
	  		break;
	  	}

	  	if (FD_ISSET(srvsock, &readset)) //is server socket?
	  	{  
			socklen_t len = sizeof(peer_addr);
			
			int peersock = accept (srvsock, (struct sockaddr * ) &peer_addr, &len);
			if (peersock < 0) {
				cout << "Error in accept (): " << strerror (errno) << endl;
			} 
			else 
			{
	      		for (auto d = mapp.begin(); d != mapp.end(); d++){
			         if((d->second.p_port) == ntohs(peer_addr.sin_port))
			         	{
			         		(d->second).p_socket = peersock;
			         		(d->second).p_last_inter = time(NULL);
			         		cout<<"[+]connection established with "<< (d->first)<<endl;
			         		break;
			         	} 
			    }
			}
		 }
		 else 
		 {	
		 	if (FD_ISSET (0, &readset))   //is stdin? --> yes
		 	{   
			 	int n=-1;
				char buf[BUFFER_SIZE];
				while(n == -1 || buf[n]!='\n'){
					read(0,&buf[++n],1);				// read msg and store it in buf
				}
				buf[n]='\0';

				if(!strcmp(buf,"exit")||!strcmp(buf,"quit")){
					exit(1);
				}

				int sockfd=-1, p_port;
				struct sockaddr_in serveraddr2,my_addr1;
				// struct hostent * server;
				//char hostname[20];

				pair <string, string> peerandMessage = parseinput (buf);
				string p_name = peerandMessage.first;
				string msg = peerandMessage.second;

				bool isPeerAvailable = false;

				map<string, peer_info>::iterator peer_itr;
				for (auto d = mapp.begin(); d != mapp.end(); d++){
					if ((d->first) == p_name) {
						//strcpy (hostname, d.second.ipadr);
						p_port = (d->second).p_port;
						peer_itr = d;
						
						isPeerAvailable = true;
						break;
					}
				}
				
				if (!isPeerAvailable) {
					cout << "!!There is no peer " << p_name<< endl;
					cout << "------------------------------------------" <<  endl;
					continue;
				}

				sockfd = (peer_itr->second).p_socket;
				
				if((peer_itr->second).p_socket == -1)     //if client socket doesn't exist
				{
					(peer_itr->second).p_socket = socket (AF_INET, SOCK_STREAM, 0);
					sockfd = (peer_itr->second).p_socket;
					if ((peer_itr->second).p_socket < 0) {
						error("ERROR opening socket");
					}

					my_addr1.sin_family = AF_INET;
				    my_addr1.sin_addr.s_addr = INADDR_ANY;
				    my_addr1.sin_port = htons(portno);

					int optval1 = 1;
					setsockopt (sockfd, SOL_SOCKET, SO_REUSEPORT, (const void *) &optval1, sizeof (int));

					//binding sockfd with port
					if (bind(sockfd, (struct sockaddr*) &my_addr1, sizeof(struct sockaddr_in)) != 0)
				        printf("Unable to bind\n");
				    
				    bzero ((char * ) &serveraddr2, sizeof (serveraddr2));
					serveraddr2.sin_family = AF_INET;
					serveraddr2.sin_addr.s_addr = INADDR_ANY;
					serveraddr2.sin_port = htons (p_port);

				    /* connect: create a connection with the server */  
					if (connect (sockfd, (struct sockaddr *) &serveraddr2, sizeof (serveraddr2)) <0) {
						cout<<(peer_itr->first)<<" is not currently connected"<<endl;
						cout << "------------------------------------------" <<  endl;
						close(sockfd);
						(peer_itr->second).p_socket = -1;
						continue;
					}

					cout<<"[+]connection established with "<< (peer_itr->first)<<endl;	
				}
				
				n=msg.length() +1;
				char message[n];
				strcpy(message, msg.c_str());
																		//send message
				(peer_itr->second).p_last_inter = time(NULL);
				flag = send(sockfd, message, strlen (message),0);
				if (flag < 0) {
					error("ERROR writing to socket");
				}
				cout << "------------------------------------------" <<  endl;	
		 	}

		
		 	else          //is stdin? --> No
		 	{
		 		for (auto d = mapp.begin(); d != mapp.end(); d++)
		 		{
		 			int peersock =  (d->second).p_socket;

					if (peersock!=-1 and FD_ISSET(peersock, &readset)) //client socket? --> yes
					{  
						(d->second).p_last_inter = time(NULL);

						char buffer[BUFFER_SIZE + 1];
						int n = recv (peersock, buffer, BUFFER_SIZE, 0);     //recv message
						
						if (n > 0) {
							buffer[n] = '\0';
							cout <<  (d->first) << " : " << buffer  << endl;
							cout << "------------------------------------------" <<  endl;
						
						} 
						else if (n == 0)                                     // read when writing fd closes
						{
							cout<<d->first<<" timed-out. Closing connection ...\n";
							cout << "------------------------------------------" <<  endl;
							close((d->second).p_socket);
							(d->second).p_socket = -1;
						}
						else
							cout << "Error in recv () : " << strerror (errno) << endl;	
					}
	
				 } 
				
			}
		}	
	}

   return 0;
}
