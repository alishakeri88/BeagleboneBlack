//**********************************************************************************************
// This code sets the following configuration for cc2520 to activate it as a receiver.
// device =  /dev/cc2520_1
// channel = 24	
// network prefix = 0x200122557788aacc
// 6lowpan IP = 0x0001
// PAN ID = 0x0022

// It receives packets and send them over USB0 network	
// to the NUC board.
// Following is the set of configurations for the socket connection
// If this program compiled with the name test you can run it as ./test 192.168.7.1 2000
// 192.168.7.1 is the address of NUC
// 2000 is the port number
//**********************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "./ioctl.h"
#include <unistd.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){	
	
// *********************************************************************************
// Configuring and turning on the radio
// Testing cc2520 driver

	int file_desc;
	file_desc = open("/dev/cc2520_1", O_RDWR);	

// Setting channel

	struct cc2520_set_channel_data chan_data;
	chan_data.channel = 24;
	ioctl(file_desc, CC2520_IO_RADIO_SET_CHANNEL, &chan_data);

// Setting address

	struct cc2520_set_address_data addr_data;
	addr_data.short_addr = 0x0001;
	addr_data.extended_addr = 0x200122557788aacc;
	addr_data.pan_id = 0x0022;
	ioctl(file_desc, CC2520_IO_RADIO_SET_ADDRESS, &addr_data);

// Setting tx power

	struct cc2520_set_txpower_data txpower_data;
	txpower_data.txpower = CC2520_TXPOWER_0DBM;
	ioctl(file_desc, CC2520_IO_RADIO_SET_TXPOWER, &txpower_data);

// Turning on the radio...

	ioctl(file_desc, CC2520_IO_RADIO_INIT, NULL);
	ioctl(file_desc, CC2520_IO_RADIO_ON, NULL);

// *********************************************************************************
// Setting the socket connection

	int sockfd, portno, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	if (argc < 3) {
		fprintf(stderr,"usage %s hostname port\n", argv[0]);
		exit(0);
	}
	portno = atoi(argv[2]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0){
		error("ERROR opening socket");
	}
	server = gethostbyname(argv[1]);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(portno);
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		error("ERROR connecting");
	}
	
// *********************************************************************************
// Reading received packets from cc2520 and send them to NUC
	
	int result = 0;
	int i = 0;
	char buf[256];
	
	
	while(i < 10000){
		bzero(buf,256);
		bzero(payload,256);
		result = read(file_desc, buf, 127);
		if (result > 0) {
			n = write(sockfd,buf,result);
		}
		i++;
	}

// *********************************************************************************
// Closing socket conection and turning radio off
	
	close(sockfd);
	ioctl(file_desc, CC2520_IO_RADIO_OFF, NULL);
	close(file_desc);
	return 0;
}
