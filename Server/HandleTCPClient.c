

#include </Users/Foodie/Desktop/4DN4/untitledfolder/4DN4 2014/VIDEO-SERVER-4DN4 folder/Video-Server folder/defineS.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void HandleTCPClient(int clntSocket)
{
    char MessageBuffer[RCVBUFSIZE];        /* Buffer of size 1024 bytes for string */
    int recvMsgSize;			   /* Size of received message */
	
    int echoStringLen;

    FILE	*fp;

    int	bytes_read, bytes_sent, length;
    int	message_number;
    int	total_bytes_sent;
    
    char command [10];

    char dir[100];
    int i,j,k;
    int strlength=0;
    DIR * d;
    char str[1000];
    char full_path[1000];
    char file_name[200];
    char path[300];
    char temp[300];
    int size=0;
    char size_string [1000];
    
    //struct flag *flag1;
  
    strcpy(str,"");

    strcpy(command,"\0");
    
    
    //root directory
    strcpy(dir,"/Users/foodie/Desktop/Above & Beyond");
    /* Initialize the message buffer for receiving CLIENT COMMANDS */
    memset(&MessageBuffer[0], 0, sizeof(MessageBuffer));   /* Zero out structure */
	
    strcpy(full_path, "/Users/foodie/Desktop/Above & Beyond/");
    
    /* Receive First COMMAND message from client */
    if ((recvMsgSize = recv(clntSocket, MessageBuffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("HandleTCPClient: did not receive message");
    
    printf("\nServer received message = %s \n", MessageBuffer);
    
    
    while(strcmp(MessageBuffer, "BYE") != 0) 
    {
       /*---------------------------------------------------------------*/
       /* check for command requested by client					*/
       /*---------------------------------------------------------------*/
        printf("HandleTCPClient: Received this COMMAND from client = %s \n", MessageBuffer);
 	
        strlength = strlen(MessageBuffer);
    
        if (strcmp(MessageBuffer, "LIST-ALL") == 0) 
        {
            
            if(flag.quit_flag)
            {
                printf("\nQUIT initiated by Server\n");
                                
                printf("\nClient closing socket\n"); 
                strcpy(str, "SERVER IN QUIT MODE");
                
                if (send(clntSocket, str, strlen(str), 0) != strlen(str))
                    DieWithError("send() failed");
                
                close(clntSocket);
                printf("\nExiting HandleTCPClient\n");
               // exit(0);
                strcpy(MessageBuffer, "BYE");
                
                continue;
                
            }
            memset(&str, 0, sizeof(str));     /* Zero out structure */

            printf("dir = %s\n", dir);
            d = opendir (dir);

            if (d) 
            {
                //fprintf (stderr, "Cannot open directory '%s': %s\n",
                  //       dir, strerror (errno));
               // exit (EXIT_FAILURE);
            
                i=-1;
                while (1) 
                {
                    strcpy(full_path,"/Users/foodie/Desktop/Above & Beyond/");
                    struct dirent * entry;

                    entry = readdir (d);
                  //  fseek(fp,0L, SEEK_END);
                  //  size = ftell(fp);
                  //  fseek(fp,0L, SEEK_SET);
                  //  itoa(size, size_string, 1);
                    if (! entry) 
                    {
                        break;
                    }
                    if(i>=1)
                    {
                        strcat(str, entry->d_name);

                        strcpy(full_path,"/Users/foodie/Desktop/Above & Beyond/");
                        strcat(full_path, entry->d_name);

                        fp = fopen(full_path, "r");

                        if  (fp == NULL)  
                                DieWithError("Cant open file for writing \n");

                        fseek(fp,0L, SEEK_END);
                        size = ftell(fp);
                        fseek(fp,0L, SEEK_SET);
                        
                        fclose(fp);
                        
                        sprintf(size_string, "%d", size);

                      //  printf("\nSize[%d] = %s\n", i, size_string);
                        strcat(str, " ");
                        strcat(str, size_string);
                        strcat(str, ",");

                    }
                    i++;
                    /* Echo the string in 'echobuffer' back to client */


                    printf ("%s\n", entry->d_name);
                }
                str[strlen(str)] = '\0';

                 printf("No. of files = %d\n", i-1);

                if (send(clntSocket, str, strlen(str), 0) != strlen(str))
                        DieWithError("send() failed");
                /* Close the directory. */
                if (closedir (d)) 
                {
                    fprintf (stderr, "Could not close '%s': %s\n",dir, strerror (errno));
                    exit (EXIT_FAILURE);
                }
            }
            else
            {
                printf ("Cannot open directory '%s'\n", dir);
                if (send(clntSocket, "Cannot open directory!!", 24, 0) != 24)
                        DieWithError("send() failed");
            }
            
            printf("\nSERVER has processed LIST-ALL request from CLIENT\n");
        }    
        else
            if (strncmp(MessageBuffer, "READ", 4) == 0) 
            {
                
                printf("\nREAD FROM SERVER...\n");

                i=0;
                while(MessageBuffer[i] != ' ')
                    i++;

                i+=1;
                j=0;
                while(MessageBuffer[i] != '\0')
                {
                    file_name[j] = MessageBuffer[i];
                    j++;
                    i++;
                }
                file_name[j] = '\0';
                strcpy(path, dir);
                strcat(path, "/");
                strcat(path, file_name);
                getfile(clntSocket, path);
                printf("\nSERVER has processed READ request from CLIENT\n");
            }
            else
                if(strncmp(MessageBuffer, "WRITE", 5)==0)
                {
                    
                    printf("\nWRITE TO SERVER...\n");
                    for(i=0; MessageBuffer[i] != ' ';i++)
                            ;

                    for(j=0, i=i+1;j<strlen(MessageBuffer);j++,i++)
                    {
                        file_name[j] = MessageBuffer[i];
                    }
                    file_name[j] = '\0';
                      
                    strcpy(dir,"/Users/foodie/Desktop/Above & Beyond");
                    
                    strcpy(temp, dir);
                    strcat(temp, "/");
                    strcat(temp, file_name);
                    temp[strlen(temp)] = '\0';
                    printf("\nfull path = %s\n", temp);
                    //get_command(sock, temp, MessageBuffer);
                    putfile(clntSocket, temp, MessageBuffer);
                    printf("\nSERVER has processed WRITE request from CLIENT\n");
                    
                    
                }
                else if (strcmp(MessageBuffer, "QUIT")==0)
                {
                    flag.quit_flag = 1;
                    printf("\nquit_flag = %d\n", flag.quit_flag);
                 
                }
                else
                {
                    printf("\nHandleTCPClient: Received request for UNKNOWN service <%s> (IGNORED)\n", MessageBuffer);
                }
        
        
        printf("\nWAITING to receive next command message from client...\n");
                
       /* Receive next COMMAND message from client */
    
        memset(&MessageBuffer, 0, sizeof(MessageBuffer));   /* Zero out old message */

        if ((recvMsgSize = recv(clntSocket, MessageBuffer, RCVBUFSIZE, 0)) < 0) 
                DieWithError("HandleTCPClient: did not receive message");
        
        if (recvMsgSize == 0) {
            printf("\nReceived message of size 0: exit\n");
            close(clntSocket);
            break;
        }
                    
  
    }  /* end while */
    
        printf("\nHandleTCPClient closing clientSocket\n");
        
	close(clntSocket);    /* Close client socket */
}
