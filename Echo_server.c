
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void){
	int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int  n, pid;

    /* First call to socket() function */
   	sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
  	 if (sockfd < 0) {
    	perror("ERROR opening socket");
      	exit(1);
   	}
   
   	/* Initialize socket structure */
   	bzero((char *) &serv_addr, sizeof(serv_addr));
   	portno = 12345;
   
   	serv_addr.sin_family = AF_INET;
   	serv_addr.sin_addr.s_addr = INADDR_ANY;
   	serv_addr.sin_port = htons(portno);
   
   	/* Now bind the host address using bind() call.*/
   	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    	perror("ERROR on binding");
      	exit(1);
   	}
      
   	/* Now start listening for the clients, here process will
      * go in sleep mode and will wait for the incoming connection
   	*/
   	listen(sockfd,5);
   	clilen = sizeof(cli_addr);
    while(1){
   		/* Accept actual connection from the client */
   		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
	
   		if (newsockfd < 0) {
    		perror("ERROR on accept");
      		exit(1);
   		}
   
   
   		pid = fork();

   		if (pid < 0){
   			perror("ERROR on fork");
			exit(1);
		}
		if (pid == 0){
			/* In child process which the handles client connection */
			close(sockfd);
			/* If connection is established then start communicating */
   			bzero(buffer,256);
   			n = read( newsockfd,buffer,255 );
   
  			if (n < 0) {
      			perror("ERROR reading from socket");
     	 		exit(1);
  	 		}
   
  		    printf("Here is the message: %s\n",buffer);
   
  			/* Write a response to the client */
   			n = write(newsockfd,"I got your message\n",19);
   
  			if (n < 0) {
      			perror("ERROR writing to socket");
      			exit(1);
   			}
			exit(0);
		}
		else
			/* In parent process which continues to listen for new clients */
			close(newsockfd);
		

		wait(NULL);
	}

	return 0;
}
