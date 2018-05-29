#include </Users/Foodie/Desktop/4DN4/untitledfolder/4DN4 2014/VIDEO-SERVER-4DN4 folder/Video-Client folder/defineC.h>

void put_command(int sock, char *str, char *MessageBufferold)  /* Error handling function */
{
    int number_messages, message_number, bytesRcvd, totalBytesRcvd, tempBufferSize;
    int bytes_written, StringLen ;

    char MessageBuffer[MESSAGESIZE];     /* Buffer for command string */
    char EchoBuffer[MESSAGESIZE];        /* Buffer for echo string */

    FILE   *fp;
    
    int	bytes_read, bytes_sent, length;
    int	total_bytes_sent;
    
    
    printf("CLIENT entering function put_command (WRITE to Server)\n");    
    printf("str = %s\n", str);

    /* in general we should receive a user-specified file */
    /* but in this code, we are receiving a specific file */
    
    printf("CLIENT Received request <WRITE> \n");

    memset(&MessageBuffer, 0, sizeof(MessageBuffer));   /* Zero out old message */

    /* Send the string "MessageBufferold" to the server */
    if (send(sock, MessageBufferold, strlen(MessageBufferold), 0) != strlen(MessageBufferold))
        DieWithError("send() sent a different number of bytes than expected");
    else
	printf("Client Sent string <%s> to server\n", MessageBuffer);
 
 
    totalBytesRcvd = 0;
    
    while (totalBytesRcvd < 4)
    {
        /* Receive up to the buffer size (minus 1 to leave space for
           a null terminator) bytes from the sender */
        if ((bytesRcvd = recv(sock, EchoBuffer, MESSAGESIZE - 1, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
        EchoBuffer[bytesRcvd] = '\0';  /* Terminate the string, just in case its not terminated */
    }

   
 printf("Client Received ACK string from server <%s>\n", EchoBuffer);    /* Print the ack */
    
    
    fp = fopen(str, "r");
    
    if(fp == NULL)
        printf("Music file %s: can't be opened!", str);
    else
        printf("File %s : opened successfully\n", str);
    
    /*------------------------------------------------------------------*/
    /* read chunks of 1024 bytes of the file into the same messagebuffer */
    /* and send them back to the client									*/
    /*------------------------------------------------------------------*/


    message_number = 0;
    total_bytes_sent = 0;
    bytes_read = 1024;
    
    memset(&MessageBuffer, 0, sizeof(MessageBuffer));   /* Zero out old message */


    while (bytes_read >= 1024) {

        bytes_read = fread(MessageBuffer, sizeof(char), 1024, fp);

        if (bytes_read == 0) {
            printf("WARNING: bytes read = 0: should I break out?\n");
           
        }

        /* keep reading and sending messages  */
        message_number = message_number + 1;

        printf("Send chunk # %d \n", message_number); 

        /* printf("Here is the message: %s\n", MessageBuffer);  */

        bytes_sent = send(sock, MessageBuffer, bytes_read, 0); 

        /* bytes_sent = bytes_read;  see what happens if we skip the send */

        /* send message to client */
        if (bytes_sent !=  bytes_read) {
                printf("send() failed: message # = %d, bytes read from file = %d, bytes sent = %d, exiting program\n", message_number, bytes_read, bytes_sent);
                close(sock);  /* free up the port */
                exit(0);
        }
        else {
              printf("SENT message %d with %d bytes\n",\
                        message_number, bytes_read);  
               printf("Total bytes sent so far = %d\n", total_bytes_sent);
             

             total_bytes_sent = total_bytes_sent + bytes_sent;

        }


    }

   /*====== last part of file sent in last iteration of while loop =====*/
    printf("Closing file\n");
    fclose(fp); 
    
}  /* end function getcommand */

