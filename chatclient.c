/* this code was adapted from examples found on
 * http://www.linuxhowtos.org/C_C++/socket.htm
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <fcntl.h>

void error(const char *msg)
{
   	fprintf(stderr, "%s\n", msg);
    exit(1);
}
void getinput(char text[], int size);
int sendall(int s, char *buf, int *len);
int validate(char text[]);
void recall(int s, char text[], int size);

int main(int argc, char *argv[])
{
    	int sockfd, portno, n;
    	struct sockaddr_in serv_addr;
    	struct hostent *server;
    	char host[28];
    	char message[10000], name[16], sername[16];
    
	//check for correct command line usage	
	if (argc < 3) 
		error("usage error: not enough arguments");
   	
	//copy host name from command line arguments
	strcpy(host, argv[1]);
	
	//get port number from command line args
	portno = atoi(argv[2]);
	
	//create a socket
    	sockfd = socket(AF_INET, SOCK_STREAM, 0);
   	if (sockfd < 0) 
   	    	error("ERROR opening socket");
    
	server = gethostbyname(host);
    	if (server == NULL) 
	{
        	error("ERROR, no such host");
    	}
    	
	bzero((char *) &serv_addr, sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
	
	//copy server address into struct
   	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
   	serv_addr.sin_port = htons(portno);//convert port to big endian order
   	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
	{
		fprintf(stderr, "ERROR connecting to port%s\n", portno);
		exit(2);
	}
 	else //upon successful connection, get users handle
	{
		printf("Connection Successful.\nPlease enter your handle: ");
		getinput(name, sizeof(name));
		
		while(validate(name) == 0)//loop until appropriate handle is entered
		{	
			printf("Try again (up to 10 letters, no spaces): ");
			getinput(name, sizeof(name));
		}
	}
	
	//exchange chat handles once so they don't need
	//to be prepended each time
	int length = strlen(name);
	if(sendall(sockfd, name, &length) < 0)
		error("error sending data");
	//get name from server
	recall(sockfd, sername, sizeof(sername));
	
	//infinite loop alternates sending/receiving with server
	while(1)
	{
		printf("%s> ", name);
		getinput(message, sizeof(message));
		while(strlen(message) > 500)//loop while msg is too long
		{
			printf("Message exceeds 500 character limit.\n");			
			printf("%s> ", name);
			getinput(message, sizeof(message));
		}
		
		//send message to server
		length = strlen(message);
		if (sendall(sockfd, message, &length) < 0)
			error("error sending data");
		
		//check if user wants to quit
		if(strcmp(message, "\\quit") == 0)
			break;	
	
		//receive response from server
		printf("%s", sername);
		fflush(stdout);
		recall(sockfd, message, sizeof(message));
		printf("%s\n", message);
		
		//check if server had sent quit message
		if(strcmp(message, "\\quit") == 0)
			break;			
	
	}
	
	printf("You will now be disconnected from the server.\n");
	close(sockfd);
    
	return 0;
}

//function to check length and for spaces of name
int validate(char text[])
{
	int i;	
	int len = strlen(text);
	
	if(len > 10)
		return 0;
	//iterate through string
	for(i = 0; i < len; i++)
	{
		if(text[i] == 32)//32 is space character
			return 0;
	}
	return 1;
}

/*Source of this function is http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html#sendman 
 * this function keeps track of bytes sent to handle partial sends through the socket
 * */
int sendall(int s, char *buf, int *len)
{
	int total = 0;        // how many bytes we've sent
	int bytesleft = *len; // how many we have left to send
	int n;

	//iterate until all the bytes are sent
	while(total < *len) {
		n = send(s, buf+total, bytesleft, 0);
		if (n == -1) { break; }
		total += n;
		bytesleft -= n;
	}

	*len = total; // return number actually sent here

	return n==-1?-1:0; // return -1 on failure, 0 on success
}

void recall(int s, char text[], int size)
{
	int n;
	memset(text, 0, size);
	n = read(s, text, size-1);
	if(n < 0)
		error("Error receiving message");

}

void getinput(char text[], int size)
{
	memset(text, 0, size);
	fgets(text, size, stdin);
	if(text[strlen(text) - 1] == '\n')
		text[strlen(text) - 1] = '\0';
}

