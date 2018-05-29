
#include </Users/Foodie/Desktop/4DN4/untitledfolder/4DN4 2014/VIDEO-SERVER-4DN4 folder/Video-Client folder/defineC.h>



void get_command(int sock, char *str, char *MessageBufferold)  /* Error handling function */
{

    int number_messages, message_number, bytesRcvd, totalBytesRcvd, tempBufferSize;
    int bytes_written, StringLen ;

    char MessageBuffer[MESSAGESIZE];     /* Buffer for command string */
    char EchoBuffer[MESSAGESIZE];        /* Buffer for echo string */
    int flag = 0;
    int status;

    FILE   *fp;



    printf("CLIENT entering function get_command (Receive File from Server)\n");
    printf("str = %s\n", str);

    /* in general we should receive a user-specified file */
    /* but in this code, we are receiving a specific file */
    fp = fopen(str, "w");

    if  (fp == NULL)
            DieWithError("Cant open file for writing \n");

   /* we know we received the string "GET"  from user */
   /* our protocol: send 2 strings to the server */
   /* "GET" and "filename" */

    memset(&MessageBuffer, 0, sizeof(MessageBuffer));   /* Zero out structure */
    strcpy(MessageBuffer, MessageBufferold);

    StringLen = strlen(MessageBuffer);          /* Determine input length */

    /* Send the string "str" to the server */
    if (send(sock, MessageBuffer, StringLen, 0) != StringLen)
    {
        DieWithError("send() sent a different number of bytes than expected");
    }
    else
	    printf("Client Sent string <%s> to server\n", MessageBuffer);

     /* wait for the "ACK" string from server before sending next message */

    totalBytesRcvd = 0;
    printf("Hello");
    while (totalBytesRcvd < 4)
    {
          printf("Hello");
        /* Receive up to the buffer size (minus 1 to leave space for
           a null terminator) bytes from the sender */
        if ((bytesRcvd = recv(sock, EchoBuffer, MESSAGESIZE - 1, 0)) <= 0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes */
        EchoBuffer[bytesRcvd] = '\0';  /* Terminate the string, just in case its not terminated */
    }


 printf("Client Received ACK string from server <%s>\n", EchoBuffer);    /* Print the ack */

 /* now send the name of the file we want to get */
 //memset(&MessageBuffer, 0, sizeof(MessageBuffer));   /* Zero out structure */
 //strcpy(MessageBuffer, "test.mov");
 StringLen = strlen(MessageBuffer);          /* Determine input length */

    /* Send the filename to the server */
	/* I disbaked this for now, sicne I am sending a pre-selecteed file */
 /*
    if (send(sock, MessageBuffer, StringLen, 0) != StringLen)
        DieWithError("send() sent a different number of bytes than expected");
    else
	    printf("Client Sent filename <%s> to server\n", MessageBuffer);
 */

 memset(&MessageBuffer, 0, sizeof(MessageBuffer));   /* Zero out structure */

 //number_messages = 20087;   /* I know that this pre-selected video has 38936 blocks */
 message_number = 1;        /* the next message I am expected is block #1 */

    printf("Start main loop to receive %d messages, of size 1024 bytes each ", number_messages);

    bytesRcvd  = 1024;

    while (bytesRcvd == 1024)
	{
		//totalBytesRcvd = 0;      /* count bytes received in each message, start with 0 */

//		while (totalBytesRcvd < 1024)  /* receive each message with 1024 bytes */
//		{
			/* Receive more bytes from the sender */

			if ((bytesRcvd = recv(sock, MessageBuffer, MESSAGESIZE , 0)) <= 0)
                        {
                            status = remove(str);

                            if( status == 0 )
                                printf("%s file deleted successfully.\n",str);
                            else
                            {
                                printf("Unable to delete the file\n");
                                perror("Error");
                            }
                            DieWithError("recv() failed or connection closed prematurely");
			}
			//totalBytesRcvd += bytesRcvd;   /* Keep tally of total bytes received in this message */
			tempBufferSize = tempBufferSize - bytesRcvd;  /* decrease buffer size  */

			/* printf(echoBuffer);      */      /* Print the echo buffer */
			printf("RECV message # %d with %d bytes\n", message_number, bytesRcvd );
//		}

		printf("RECV() chunk # %d\n", message_number);

		/* write the chink to a file */
		bytes_written = fwrite(MessageBuffer, sizeof(char), MESSAGESIZE, fp);

		if (bytes_written < 1024) {

			printf("WARNING: bytes written = 0: break out\n");
			break;
		}
		//if(flag)
                //    break;
		/* we are now expecting the next message */
		message_number = message_number + 1;

    }  /* while message_number < number_messages */

    printf("\nClient Finished <READ> command and received Big File\n");

    fclose(fp);


}  /* end function getcommand */
