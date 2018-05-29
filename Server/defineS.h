/* 
 * File:   define.h
 * Author: Fahd
 *
 * Created on March 17, 2014, 11:47 PM
 */

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

#include <pthread.h> /* for POSIX threads */

void *ThreadMain(void *arg); /* Main program of a thread */

void DieWithError(char *errorMessage);  /* Error handling function */
void getfile(int clntSocket, char *path);
void putfile(int sock, char *str, char *MessageBuffer);

void get_command(int sock, char *str, char *MessageBuffer);
void put_command(int sock, char *str, char *MessageBuffer);


int AcceptTCPConnection(int servSock);
int CreateTCPServerSocket(unsigned short port);

void HandleTCPClient(int clntSock);

 struct flag
    {
        int quit_flag;
    }flag;


#endif	/* DEFINE_H */

