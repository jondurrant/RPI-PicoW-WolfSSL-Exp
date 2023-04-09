/*
 * TCPTransport.h
 *
 * TCP Socket transport to provide as a transport layer to FreeRTOS MQTT Agent library
 *
 *  Created on: 31 Jul 2022
 *      Author: jondurrant
 */

#ifndef TCPTRANSPORT_H_
#define TCPTRANSPORT_H_

#define TCP_TRANSPORT_WAIT 10000

#include "Transport.h"

extern "C" {
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "lwip/ip4_addr.h"
#include "lwip/ip_addr.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"

}


class TCPTransport : public Transport {
public:
	/***
	 * Constructor
	 */
	TCPTransport();

	/***
	 * Destructor
	 */
	virtual ~TCPTransport();

	/***
	 * Connect to remote TCP Socket
	 * @param host - Host address
	 * @param port - Port number
	 * @return true on success
	 */
	bool transConnect(const char * host, uint16_t port);

	/***
	 * Get status of the socket
	 * @return int <0 is error
	 */
	int status();

	/***
	 * Close the socket
	 * @return true on success
	 */
	bool transClose();


	/***
	 * Send bytes through socket
	 * @param pBuffer - Buffer to send from
	 * @param bytesToSend - number of bytes to send
	 * @return number of bytes sent
	 */
	int32_t transSend(const void * pBuffer, size_t bytesToSend);

	/***
	 * Send
	 * @param pBuffer
	 * @param bytesToRecv
	 * @return
	 */
	int32_t transRead( void * pBuffer, size_t bytesToRecv);


private:

	/***
	 * Connect to socket previously stored ip address and port number
	 * @return true if socket openned
	 */
	bool transConnect();

	/***
	 * Call back function for the DNS lookup
	 * @param name - server name
	 * @param ipaddr - resulting IP address
	 * @param callback_arg - poiter to TCPTransport object
	 */
	static void dnsCB(const char *name, const ip_addr_t *ipaddr, void *callback_arg);

	/***
	 * Called when DNS is returned
	 * @param name - server name
	 * @param ipaddr - ip address of server
	 * @param callback_arg - this TCPtransport object
	 */
	void dnsFound(const char *name, const ip_addr_t *ipaddr, void *callback_arg);

	//Socket number
	int xSock = 0;

	// Port to connect to
	uint16_t xPort=80;

	// Remote server IP to connect to
	ip_addr_t xHost;

	// Remote server name to connect to
	char xHostName[80];

	// Semaphore used to wait on DNS responses
	SemaphoreHandle_t xHostDNSFound;


};

#endif /* TCPTRANSPORT_H_ */
