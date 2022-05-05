/*<----CLIENT SERVER PROGRAMMING---_>
<----CLIENT CODE---->
<----VIDISH SHARMA_110036779---->
<----SNEHA ARORA_110061291---->
*/
// Starting off with defining the necessary headerfiles
#include <sys/socket.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

//Defining the BufferSize as 80 and Port as 4040 in our case along with a structure named "sockaddr"// 
#define BUFFER_SIZE 100
#define PORT 8080
#define SA struct sockaddr

//Defining the the function named clientFunction that takes in arguments namely, socket file descripter and the arguments that are passed from the user to the terminal.( argv = array of pointers).
void clientFunction(int socket_fd, char *argv[]) {
	// Buffer size defined as 100 prior.
	char buffer[BUFFER_SIZE]; 
	bzero(buffer, sizeof(buffer));// Using bzero, can place the nbyte null bytes in the string Buffer.
	write(socket_fd, argv[3], sizeof(buffer));//Writing to the pipe using the socket file descriptor.
	write(socket_fd, argv[1], sizeof(buffer));
	bzero(buffer, sizeof(buffer));
	read(socket_fd, buffer, sizeof(buffer));//reading from the pipe from the socket file descriptor.
	
	int fdpipe[2]; // Defining file desciptor for piping.
	if(pipe(fdpipe) < 0) {
		// Checking if the pipe is initialised with fd >0 otherwise , it will show the corresponding error.
		perror("\nError while inititalizing pipe.");
		exit(1); 
	}
	pid_t pid;
	pid = fork();
	if(pid == 0){ 
		// checking if there are child process created as child does not use fd[0]
		close(fdpipe[0]);
		execlp("grep", "grep", "-w", argv[1], argv[2], NULL); // Enabling to execute the c commands over the linux terminal.
	} else {
		char result[BUFFER_SIZE]; //
		close(fdpipe[1]); //since parent does not use fd[1]
		printf("\n\nThe output of this Client as:\n");
		// Reading from the pipe.
		read(fdpipe[0], &result, BUFFER_SIZE);
		printf("\nThe output of this Server as:\n");
		printf("\n%s: ", argv[3]);
		
		for(int i = 0 ; i < sizeof(buffer); i++) {
		    if(buffer[i] == '\n')
		        printf("\n%s: ", argv[3]);
		    else
		        printf("%c", buffer[i]);
		}
	}
}

int main(int argc, char *argv[])
{
	printf("Welcome to Client-Server Programming!!\n");
	int socket_fd, connection_fd; // Initialising socket and the connection file descriptors.
	struct sockaddr_in serverAddress, client;// Defining the structure sockaddr_in and IPaddress and Port are passed as parameters once casted. Herein the structure handles the addresses of network traffic in order to deal with network addresses.

	if(argc !=4) {
		printf("There is an Error reported, please specify the pattern in the given syntax");
		exit(0);
	}

	// socket create and verification
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);// herein the socket call is creating a socket in the specified domain , type and the protocol. The following creates a stream socket with the Internet Domain.This call results in a stream socket with the TCP protocol providing the underlying communication.
	
	if (socket_fd == -1) { //In order for the client to connect it should be somewhere from 1 to 65535 as 0 is reserved. Once it is , the socket is successfully created and ready to be accepted.
		fprintf(stderr,"Error while creating socket.\n");
		exit(0);
	} else {
		printf("Successfully created the socket.\n");
	}
	bzero(&serverAddress, sizeof(serverAddress));// using Bzero we can place the nbyte null bytes in the string erasing the data in the n bytes of the memory starting at the position pointed to by the serverAddress here.

	// assign IP, PORT
	// The address family for the transport address it is always set to AF_NET
	//sin_addr specifies the IP address of the socket and the other assigns the port address and translating the integer short type from host byte to network byte order in which the MSB will be first.
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("137.207.82.52");
	serverAddress.sin_port = htons(PORT);

	// The client requests services from the server by initiating a connect call to the socket.The server binds a socket to a well-known address associated with the service and blocks on its socket for a connect request
	if (connect(socket_fd, (SA*)&serverAddress, sizeof(serverAddress)) != 0) {
		fprintf(stderr, "Error while connecting to the server.\n");
		exit(0);
	} else {
		printf("Successfully connected to the server.\n");
	}

	// the function can now work with the unique descriptor of the particular socket and the arguments passed on the terminal
	clientFunction(socket_fd, argv);
	
	close(socket_fd);// We shutdown a socket
}

/* REFERENCES:
The following code is referred from the below given link.
--> https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/?ref=lbp
*/