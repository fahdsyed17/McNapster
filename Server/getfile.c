#include </Users/Foodie/Desktop/4DN4/untitledfolder/4DN4 2014/VIDEO-SERVER-4DN4 folder/Video-Server folder/defineS.h>


/*---------------------------------------------------------------*/
/* Add a global ALL_FILES data structures for our application	 */
/*---------------------------------------------------------------*/
struct {
        int valid;
        char string[64];  /* store the file name here  */
        int  bytes;
} ALL_FILES[16];


void getfile(int clntSocket, char *path) 
{
char MessageBuffer[RCVBUFSIZE1 +1];        /* Buffer of size 1024 bytes for string */
int recvMsgSize;		  	/* Size of received message */
		
FILE	*fp;
	
int	bytes_read, bytes_sent, length;
int	message_number;
int	total_bytes_sent;

  
memset(&ALL_FILES[0], 0, sizeof(ALL_FILES));   /* Zero out structure */

/* add one known file to the table of known files */
ALL_FILES[0].valid = 1;
    
//strcpy(ALL_FILES[0].string, "/Users/foodie/Desktop/kaskade-atmosphere.mp3"); 
strcpy(ALL_FILES[0].string, path); 


    printf("SERVER Received request <READ> (Send file to CLient)\n");

    /* send an ACK string back to client */

    memset(&MessageBuffer, 0, sizeof(MessageBuffer));   /* Zero out structure */
    strcpy(MessageBuffer, "ACK");

    bytes_sent = send(clntSocket, MessageBuffer, 4, 0); 

    /* send message to client */
    if (bytes_sent !=  4) {
            printf("sending ACK string failed ! exit  program\n");
            close(clntSocket);  /* free up the port */
            exit(0);
    }


   /* I should search my directory or table for the correct file  */
   /* however, for now I will just open the one file that I know about */

   fp = fopen(ALL_FILES[0].string, "r");

    if (fp == NULL)
        printf("Music file %s : cant be opened !!\n", ALL_FILES[0].string);
    else
        printf("File %s : opened successfully\n", ALL_FILES[0].string);

    /*------------------------------------------------------------------*/
    /* read chunks of 1024 bytes of the file into the same messagebuffer */
    /* and send them back to the client									*/
    /*------------------------------------------------------------------*/

    /* while ( (bytes_read = fread(MessageBuffer, sizeof(char), 1024, fp)) == 1024) {  */

    message_number = 0;
    total_bytes_sent = 0;
    bytes_read = 1024;

    while (bytes_read >= 1024) {

        bytes_read = fread(MessageBuffer, sizeof(char), 1024, fp);

        if (bytes_read == 0) {
            printf("WARNING: bytes read = 0: should I break out?\n");

           
        }

        /* keep reading and sending messages  */
        message_number = message_number + 1;

        printf("Send chunk # %d \n", message_number); 

        /* printf("Here is the message: %s\n", MessageBuffer);  */

        bytes_sent = send(clntSocket, MessageBuffer, bytes_read, 0); 

        /* bytes_sent = bytes_read;  see what happens if we skip the send */

        /* send message to client */
        if (bytes_sent !=  bytes_read) {
                printf("send() failed: message # = %d, bytes read from file = %d, bytes sent = %d, exiting program\n", message_number, bytes_read, bytes_sent);
                close(clntSocket);  /* free up the port */
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

}  /* end function getfile */
