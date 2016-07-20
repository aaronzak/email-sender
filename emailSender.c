/*
* CS 176A Programming Assignment 3
*
* The command line arguments should be:
* ./EmailSender <recipient address>
*
* NOTE:
* 1- I would like you to hardcode the following:
*	a- Sender address: "MAIL FROM".
*	b- Message you are sending.
* 2- Pass the recipient address "RCPT TO" as a command line argument.
* 3- INCLUDE comments in the code to show what is going on (not too detailed).
* 4- INCLUDE a README file if something did not run properly. That will help you receive partial credit.
*
*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>
#include <string.h>

#define SMTP_PORT 25
#define MAXLINE 1024 			// NOTE: Can be modified to any size you want.

char *Server_IP = "128.111.53.4"; 	// NOTE: IP of the mail server after running "nslookup -type=MX cs.ucsb.edu".  Hence you will need to change this value.

void main (int argc, char ** argv)
{
	char sendline[MAXLINE], recvline[MAXLINE];
	char recipient_address[MAXLINE]; // "RCPT TO" address.
	char sender_address[MAXLINE];	 // "MAIL FROM" adress.
	char *temp;
	
	if (argc != 2)
	{
		printf("usage: ./EmailSender <recipient address,\"RCPT TO\">\n");
		exit(0);
	}
	strcpy(recipient_address, argv[1]);
			
	/* Establish a TCP connection with the main server */
	int sockfd;
	struct sockaddr_in serveraddr;

	sockfd=socket(AF_INET, SOCK_STREAM,0); // create a socket for sockfd

	
	memset(&serveraddr, '0', sizeof(serveraddr)); // reset the serveraddr
	//   	 memset(sendBuff, '0', sizeof(sendBuff)); 

	if(sockfd==-1) // check if the socket can open
	{
		printf("cannot open sockfd");
		return -1;
	}
	
	serveraddr.sin_family = AF_INET; // set the type of internet family
	serveraddr.sin_addr.s_addr = inet_addr(Server_IP); // set the server address
	serveraddr.sin_port = htons(SMTP_PORT); //set the port number
	connect(sockfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr));// set the connection
	
	
	
	
	/* Read greeting from the server */
	read(sockfd, recvline, MAXLINE); // read from the server
	printf("%s\n", recvline);	
	temp = strtok(recvline, " ");
	if (strcmp(temp, "220")!=0)
	{
		printf("220 reply not received from server.\n");
		exit(0);
	}
	
	/* Send HELO command and get server response */
	strcpy(sendline, "HELO alice\r\n"); // say hello to alice
	printf("%s\n", sendline);
	write(sockfd, sendline, strlen(sendline));
	
	read(sockfd, recvline, MAXLINE); // read in the 250 protocol
	printf("%s\n", recvline); // print the 250 protocol
	temp = strtok(recvline, " ");
	if (strcmp(temp, "250")!=0)
	{
		printf("250 reply not received from server.\n");
		exit(0);
	}
	memset( recvline, '\0', sizeof(recvline) );
	// Send MAIL FROM command.
	strcpy(sender_address, "MAIL FROM: azakhor@cs.ucsb.edu\r\n"); 	// NOTE: replace address with your own.

	 printf("%s\n", sender_address);
	write(sockfd, sender_address, strlen(sender_address));
	
	read(sockfd, recvline, MAXLINE); // read in the 250 protocol
	printf("%s\n", recvline); // print the 250 protocol
	temp = strtok(recvline, " ");
	if (strcmp(temp, "250")!=0)
	{
		printf("250 reply not received from server.\n");
		exit(0);
	}
	memset( recvline, '\0', sizeof(recvline) );
		
	// Send RCPT TO command.
	
	
	strcpy(sendline, "RCPT TO: ");
	strcat(sendline, recipient_address);
	char spaceline[5];
	strcpy(spaceline, "\r\n");
	strcat(sendline,spaceline);

	printf("%s\n", sendline);
	write(sockfd, sendline, strlen(sendline));

	read(sockfd, recvline, MAXLINE); // read in the 250 protocol
	printf("%s\n", recvline); // print the 250 protocol
	temp = strtok(recvline, " ");
	if (strcmp(temp, "250")!=0)
	{
		printf("250 reply not received from server.\n");
		exit(0);
	}
       
	memset( recvline, '\0', sizeof(recvline) );

	// Send DATA command.
	char data[15];
	strcpy(data,"DATA\r\n");
	printf("%s\n", data);
	write(sockfd, data, strlen(data));
	
	read(sockfd, recvline, MAXLINE); // read in the 354 protocol
	printf("%s\n", recvline); // print the 354 protocol
	temp = strtok(recvline, " ");
	if (strcmp(temp, "354")!=0)
	{
		printf("354 reply not received from server.\n");
		exit(0);
	}
	// Send message data.

	
	char messageTo[150];
	strcpy(messageTo,"Subject: CS176A\r\n");
	printf("%s\n", messageTo);
	write(sockfd, messageTo, strlen(messageTo));
	char messageFrom[150];
	strcpy(messageFrom,"From: Aaron\r\n");
	printf("%s\n", messageFrom);
	write(sockfd, messageFrom, strlen(messageFrom));	

	char messageToo[150];
	strcpy(messageToo,"To: Friend\r\n");
	printf("%s\n", messageToo);
	write(sockfd, messageToo, strlen(messageToo));
	
	



	char theBody[2000];
	strcpy(theBody,"I love Networking!\r\n");
	printf("%s\n", theBody);
	write(sockfd, theBody, strlen(theBody));


	// End with a single period.
	write(sockfd, ".\r\n", 6);
	// Send QUIT command.
	char quitEmail[15];
	strcpy(quitEmail,"QUIT\r\n");
	printf("%s\n", quitEmail);
	write(sockfd, quitEmail, strlen(quitEmail));


	memset( recvline, '\0', sizeof(recvline) );

	read(sockfd, recvline, MAXLINE); // read in the 354 protocol
	printf("%s\n", recvline); // print the 354 protocol
	temp = strtok(recvline, " ");
	
	exit(0);	
}  
