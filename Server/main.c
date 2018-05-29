//#include "TCPEchoServer.h" /* TCP echo server includes */

#include </Users/Foodie/Desktop/4DN4/untitledfolder/4DN4 2014/VIDEO-SERVER-4DN4 folder/Video-Server folder/defineS.h>

void *ThreadMain(void *arg); /* Main program of a thread */

/* Structure of arguments to pass to client thread */
struct ThreadArgs
{
        int clntSock;
};

int main(int argc, char *argv[])
{
    int servSock;/* Socket descriptor for client */
    int clntSock;/* Socket descriptor for server */

    unsigned short echoServPort; /* Server port */
    pthread_t threadID; /* Threaad ID from pthread_create()*/
    struct ThreadArgs *threadArgs;/*Pointer to argument structure for thread*/

    
    if(argc != 2)
    {
        fprintf(stderr, "Usage: %s <SERVER PORT>\n",argv[0]);
        exit(1);
    }

    echoServPort = atoi(argv[1]);
    printf("\n------------------- entering TCPEchoServer-Threads --------------------\n\n");
    printf("\nStarting TCPEchoServer-Threads, Server Port = %d \n\n", echoServPort);
    
    servSock = CreateTCPServerSocket(echoServPort);
    
    printf("\nCreating TCPEchoServer Socket # %d\n\n", servSock);


    for(;;)
    {
        printf("\nServer Process: Calling blocking Accept-TCP-Connection() \n\n");

        clntSock = AcceptTCPConnection(servSock);
	
        printf("\nServer Process: Create separate memory for client argument \n\n");

        /* Create separate memory for client argument*/
        if((threadArgs = (struct ThreadArgs *) malloc(sizeof(struct ThreadArgs))) == NULL)
            DieWithError("malloc() failed");

        threadArgs -> clntSock = clntSock;

        printf("\nCreate client thread\n\n");
        /*Create client thread*/

        if(pthread_create(&threadID, NULL, ThreadMain, (void *) threadArgs) != 0)
            DieWithError("pthread_create failed");

        printf("with thread %ld \n", (long int) threadID);
        
        
            
    }
}

void *ThreadMain(void *threadArgs)
{
    int clntSock;
    
    pthread_detach(pthread_self());
    
    clntSock = ((struct ThreadArgs *) threadArgs) -> clntSock;
    free(threadArgs);
    
    printf("\nSERVER Handling CLIENT...\n");
    HandleTCPClient(clntSock);
    
    return(NULL);
}