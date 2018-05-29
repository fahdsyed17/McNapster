
#ifndef DEFINE_H
#define	DEFINE_H


#include <stdio.h>      /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <unistd.h>     /* for close() */
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>

#define RCVBUFSIZE1 1024   /* Size of receive buffer at server */

#define MESSAGESIZE 1024   /* Size of receive buffer */
#define ECHOMAX 1024        /* Longest string to echo */

#define RCVBUFSIZE 1024   /* Size of receive buffer at server */



void DieWithError(char *errorMessage);  /* Error handling function */


void get_command(int sock, char *str, char *MessageBuffer);
void put_command(int sock, char *str, char *MessageBuffer);


 struct q_flag
    {
        int flag;
    }q_flag;


#endif	/* DEFINE_H */

