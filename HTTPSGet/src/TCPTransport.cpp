/*
 * TCPTransport.cpp
 *
 *  TCP Socket transport to provide as a transport layer to FreeRTOS MQTT Agent library
 *
 *
 *  Created on: 31 Jul 2022
 *      Author: jondurrant
 */

#include "TCPTransport.h"

#include <stdlib.h>
#include "pico/stdlib.h"
#include <errno.h>



/***
 * Constructor
 */
TCPTransport::TCPTransport(){
	xHostDNSFound = xSemaphoreCreateBinary(  );
}

/***
 * Destructor
 */
TCPTransport::~TCPTransport() {
	// NOP
}


/***
 * Send bytes through socket
 * @param pNetworkContext - Network context object from MQTT
 * @param pBuffer - Buffer to send from
 * @param bytesToSend - number of bytes to send
 * @return number of bytes sent
 */
int32_t TCPTransport::transSend(const void * pBuffer, size_t bytesToSend){
	uint32_t dataOut;

	//debugPrintBuffer("TCPTransport::transSend", pBuffer, bytesToSend);

	dataOut = write(xSock,(uint8_t *)pBuffer, bytesToSend);
	if (dataOut != bytesToSend){
		printf("Send failed %d\n", dataOut);
	}
	return dataOut;
}


/***
 * Send
 * @param pNetworkContext
 * @param pBuffer
 * @param bytesToRecv
 * @return
 */
int32_t TCPTransport::transRead(void * pBuffer, size_t bytesToRecv){
	int32_t dataIn=0;

	dataIn = read(xSock, (uint8_t *)pBuffer, bytesToRecv);

	if (dataIn < 0){
		if (errno == 0){
			dataIn = 0;
		}
	}

	//printf("Read(%d)=%d\n", bytesToRecv, dataIn);

	//printf("transRead(%d)=%d\n", bytesToRecv, dataIn);
	return dataIn;
}


/***
 * Connect to remote TCP Socket
 * @param host - Host address
 * @param port - Port number
 * @return true on success
 */
bool TCPTransport::transConnect(const char * host, uint16_t port){
	err_t res = dns_gethostbyname(host, &xHost, TCPTransport::dnsCB, this);

	strcpy(xHostName, host);
	xPort = port;

	if (xSemaphoreTake(xHostDNSFound, TCP_TRANSPORT_WAIT) != pdTRUE){
		printf("DNS Timeout on Connect: %s, %d", host, res);
		//return false;
	}

	return transConnect();
}


/***
 * Connect to socket previously stored ip address and port number
 * @return true if socket openned
 */
bool TCPTransport::transConnect(){
	struct sockaddr_in serv_addr;


	xSock = socket(AF_INET, SOCK_STREAM, 0);
	if (xSock < 0){
		printf("ERROR opening socket\n");
		return false;
	}


	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(xPort);
	memcpy(&serv_addr.sin_addr.s_addr, &xHost, sizeof(xHost));

	int res = connect(xSock,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
	if (res < 0){
		char *s = ipaddr_ntoa(&xHost);
		printf("ERROR connecting %d to %s port %d\n",res, s, xPort);
		return false;
	}


	printf("Connect success\n");
	return true;
}


/***
 * Get status of the socket
 * @return int <0 is error
 */
int TCPTransport::status(){
	int error = 0;
	socklen_t len = sizeof (error);
	int retval = getsockopt (xSock, SOL_SOCKET, SO_ERROR, &error, &len);
	return error;
}

/***
 * Close the socket
 * @return true on success
 */
bool TCPTransport::transClose(){
	closesocket(xSock);
	return true;
}

/***
 * Call back function for the DNS lookup
 * @param name - server name
 * @param ipaddr - resulting IP address
 * @param callback_arg - poiter to TCPTransport object
 */
void TCPTransport::dnsCB(const char *name, const ip_addr_t *ipaddr, void *callback_arg){
	TCPTransport *self = (TCPTransport *) callback_arg;
	self->dnsFound(name, ipaddr, callback_arg);
}


/***
 * Called when DNS is returned
 * @param name - server name
 * @param ipaddr - ip address of server
 * @param callback_arg - this TCPtransport object
 */
void TCPTransport::dnsFound(const char *name, const ip_addr_t *ipaddr, void *callback_arg){
	memcpy(&xHost, ipaddr, sizeof(xHost));

	printf("DNS Found %s copied to xHost %s\n", ipaddr_ntoa(ipaddr), ipaddr_ntoa(&xHost));
	xSemaphoreGiveFromISR(xHostDNSFound, NULL );
}




