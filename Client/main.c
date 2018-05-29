/*------------------------------------------------------*/
/* modified TCPEchoClient.c, to get/put music videos	*/
/* COE  4DN4 - Advanced Internet Communications			*/
/* Prof. Ted Szymanski									*/
/* Thurs, Jan. 23, 2014									*/
/*------------------------------------------------------*/

#include </Users/Foodie/Desktop/4DN4/untitledfolder/4DN4 2014/VIDEO-SERVER-4DN4 folder/Video-Client folder/defineC.h>



int main(int argc, char *argv[])
{
    int sock;                        /* Socket descriptor */
    struct sockaddr_in echoServAddr; /* Echo server address */
    unsigned short echoServPort;     /* Echo server port */
    char *servIP;                    /* Server IP address (dotted quad) */
    
    char MessageBuffer[RCVBUFSIZE];  /* Buffer for client command */
    char   PortBuffer[128];          /* buffer for server port string */
     
    unsigned int StringLen;          /* Length of string to send */
    int bytesRcvd, totalBytesRcvd;   /* Bytes read in single recv() 
                                        and total bytes read */    
    char command[10];
    char curr_dir[300];
    char files[10][200];
    char file_name[200];
    char temp[200];
    int Len;
    int i,j, k, quit_flag = 0;
    
    int number_messages, message_number, tempBufferSize, bytes_written;
	
    strcpy(temp, "\0");
    if ((argc != 2))    /* Test for correct number of arguments */
    {
       fprintf(stderr, "Usage: %s <Server IP> \n",
            argv[0]);
       exit(1);
    }
  
    
    
 
    
    
    servIP = argv[1];             /* First arg: server IP address (dotted quad) */
    
    /* prompt user to enter server port number on keyboard */
    printf("Enter Port : ");
    gets(PortBuffer);  
  
    echoServPort = atoi(PortBuffer); /* Use given port, if any */
   
    printf("Client calling server on port %d\n", echoServPort);
  
    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError("socket() failed");

    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr));     /* Zero out structure */
    echoServAddr.sin_family      = AF_INET;             /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    echoServAddr.sin_port        = htons(echoServPort); /* Server port */

    /* Establish the connection to the echo server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError("connect() failed ");
    else
	    printf("Client connected with server\n");
    
    
    
    // CLIENT Working Directory
    
    strcpy(curr_dir, "/Users/foodie/Desktop/abcd");
    
   // incase of user input we can use the two lines below: 
   // printf("Client's current Working Directory = ");
   // gets(curr_dir);
    
    memset(&MessageBuffer, 0, sizeof(MessageBuffer));     /* Zero out structure */
     
    printf("Enter Client Command : ");
    gets(MessageBuffer);  
    
    
    while(strcmp(MessageBuffer, "BYE") != 0) {

       
        if(strcmp(MessageBuffer, "LIST-ALL") == 0)
        {
            
            Len = strlen(MessageBuffer);          /* Determine input length */

             /* Send the string to the server */
            if (send(sock, MessageBuffer, Len, 0) != Len)
                    DieWithError("send() sent a different number of bytes than expected");

            printf("Client sent message to server: %s", MessageBuffer);
            
            printf("\n");

            // printf("echoString: %s\n", echoString);

             
            memset(&MessageBuffer, 0, sizeof(MessageBuffer));     /* Zero out structure */
            

            bytesRcvd = recv(sock, MessageBuffer, RCVBUFSIZE - 1, 0);

            if (bytesRcvd  <= 0)
                DieWithError("recv() failed or connection closed prematurely");

            printf("Client Receives string '%s'\n", MessageBuffer);

            if(strcmp(MessageBuffer, "SERVER IN QUIT MODE")==0)
            {
                strcmp(MessageBuffer, "BYE");
                break;
            }
            else
            if(strcmp(MessageBuffer, "Cannot open directory!!") != 0)
            {
                MessageBuffer[strlen(MessageBuffer)] = '\0';
                i=0;
                j=0;
              
                for(k=0;k<strlen(MessageBuffer)-1;k++)
                {
                    while(MessageBuffer[k] != ',')
                    {
                        files[i][j] = MessageBuffer[k];
                        j++; 
                        k++;
                    }
                   

                    files[i][j] = '\0';
               
                    i++;
                    j=0;
                }
          
                printf("\nFiles available to download from SERVER's Root Directory: ");
                for(j=0;j< i;j++)
                        printf("\n[%d]: %s", j+1, files[j]);

            }
          
            printf("\n");    /* Print a final linefeed */ 


        }
        else if (strncmp(MessageBuffer, "READ", 4) == 0) 
        {
            
                if(q_flag.flag)
                {
                    printf("\nSERVER IN QUIT MODE\n");
                    strcpy(MessageBuffer, "BYE");
                    continue;
                }
                else
                {
        
                    for(i=0; MessageBuffer[i] != ' ';i++)
                        ;

                    for(j=0, i=i+1;j<strlen(MessageBuffer);j++,i++)
                    {
                        file_name[j] = MessageBuffer[i];
                    }
                    file_name[j] = '\0';


                    strcpy(curr_dir, "/Users/foodie/Desktop/abcd");

                    strcpy(temp, curr_dir);
                    strcat(temp, "/");
                    strcat(temp, file_name);
                    temp[strlen(temp)] = '\0';
                    printf("full path = %s\n", temp);
                    get_command(sock, temp, MessageBuffer);
                    printf("\nREAD Command Processed...\n");
                }
               
                printf("\nquit_flag in read loop = %d\n", q_flag.flag);
        }
        else if (strncmp(MessageBuffer, "WRITE", 5) == 0) 
        {
             if(q_flag.flag)
            {
                 printf("\nSERVER IN QUIT MODE\n");
                 strcpy(MessageBuffer, "BYE");
                 continue;
            }
            else
            {
                printf("\nWRITE initiated by Client...\n");
                for(i=0; MessageBuffer[i] != ' ';i++)
                        ;

                for(j=0, i=i+1;j<strlen(MessageBuffer);j++,i++)
                {
                    file_name[j] = MessageBuffer[i];
                }
                file_name[j] = '\0';


                strcpy(curr_dir, "/Users/foodie/Desktop/abcd");

                strcpy(temp, curr_dir);
                strcat(temp, "/");
                strcat(temp, file_name);
                temp[strlen(temp)] = '\0';
                printf("\nfull path = %s\n", temp);

                put_command(sock, temp, MessageBuffer);
                printf("\nWRITE Command Processed...\n");

            }
          
        }
        else if(strcmp(MessageBuffer, "QUIT") == 0)
        {
            q_flag.flag = 1;
            printf("\nquit_flag in quit loop = %d\n", q_flag.flag);

             if (send(sock, "QUIT", 4, 0) != 4)
                DieWithError("send() sent a different number of bytes than expected");
             else
                printf("Client Sent string <%s> to server\n", MessageBuffer);
        }
        else 
            printf("\nUnknown command:  <%s>  ignored\n", MessageBuffer);

        //printf("root - dir = %s\n", root_dir);
        
        memset(&MessageBuffer, 0, sizeof(MessageBuffer));     /* Zero out structure */
       // memset(&root_dir, 0, sizeof(root_dir));
        /* get the next command and repeat loop */
        
        
        printf("\nEnter Next Client Command : ");
        gets(MessageBuffer);   
        
         for(i=0;i<10;i++)
            strcpy(files[i], "\0");
     
        
        
    }  /* end while loop */
     
   
    printf("Client closing socket\n");    
   
    close(sock);
    exit(0);
}
