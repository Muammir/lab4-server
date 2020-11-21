#include<stdio.h>
#include<string.h>      //strlen
#include<sys/socket.h>
#include<arpa/inet.h>   //inet_addr
#include<unistd.h>      //write
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>

int main(int argc , char *argv[])
{
	int socket_desc , new_socket , c;
	struct sockaddr_in server ;
	struct sockaddr_in Baruserver;
	char buffer[2000], server_reply[2000];
	pid_t Childpid;
	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
		exit(1);
	}
	
	//Prepare the sockaddr_in structure
	memset(&server, '\0', sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons( 8888 );
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		puts("bind failed");
		exit(1);
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , 3);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	while(new_socket = accept(socket_desc, (struct sockaddr *)&Baruserver, (socklen_t*)&c)){
	if (new_socket<0)
		{
		perror("accept failed");
		exit(1);
		}	
	puts("Connection accepted");
	if((Childpid = fork()) == 0){
			close(socket_desc);

			while(1){
				recv(new_socket, buffer, 2000, 0);
				if(strcmp(buffer, ":exit") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(Baruserver.sin_addr), ntohs(Baruserver.sin_port));
					break;
				}else{
					printf("Client: %s\n", buffer);
					send(new_socket, buffer, strlen(buffer), 0);
					bzero(buffer, sizeof(buffer));
				}
//				printf("Server: \t");
  //              		scanf("%s", &server_reply[0]);
    //            		send(socket_desc, server_reply, strlen(server_reply), 0);
			}
		}
//	send(new_socket, msg, 2000, 0);
//	recv(new_socket, buffer, 2000, 0);
//	printf("message from client: %s\n", buffer);

	}
	close(socket_desc);
	return 0;
}
