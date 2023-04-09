/*
 * TLSTransBlock.h
 *
 * TLS Transport - Blocking socket except for transRead of 1 byte which will not block
 * Written for the FreeRTOS coreMQTT transport requirements
 *
 * Does not set a certificate or enforce any certificate checks of the server
 *
 *  Created on: 3 Apr 2023
 *      Author: jondurrant
 */

#ifndef _TLSTRANSBLOCK_H_
#define _TLSTRANSBLOCK_H_

#define TLS_TRANSPORT_WAIT 10000

#include "Transport.h"

extern "C" {
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "lwip/ip4_addr.h"
#include "lwip/ip_addr.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"

#include "wolfssl/ssl.h"

}



class TLSTransBlock :  public Transport{
public:
	TLSTransBlock();
	virtual ~TLSTransBlock();

	/***
	 * Connect to remote TLS Socket
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
	int32_t transSend( const void * pBuffer, size_t bytesToSend);

	/***
	 * Send
	 * @param pBuffer
	 * @param bytesToRecv
	 * @return
	 */
	int32_t transRead(void * pBuffer, size_t bytesToRecv);

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
	 * @param callback_arg - poiter to TLSTransport object
	 */
	static void dnsCB(const char *name, const ip_addr_t *ipaddr, void *callback_arg);

	/***
	 * Called when DNS is returned
	 * @param name - server name
	 * @param ipaddr - ip address of server
	 * @param callback_arg - this TLStransport object
	 */
	void dnsFound(const char *name, const ip_addr_t *ipaddr, void *callback_arg);


	/***
	 * Send function to connect WolfSSL to the local socket function
	 * @param ssl - wolf ssl data
	 * @param buff - send buffer
	 * @param sz - size to send
	 * @param ctx Wolfssl context
	 * @return
	 */
	static int IOSend(WOLFSSL* ssl, char* buff, int sz, void* ctx);

	/***
	 * Receive function to connect WolfSSL to the local socket
	 * @param ssl - wolf ssl data
	 * @param buff - buffer to write into
	 * @param sz - max size of the buffer
	 * @param ctx - wolfssl context
	 * @return
	 */
	static int IORecv(WOLFSSL* ssl, char* buff, int sz, void* ctx);



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

	WOLFSSL_CTX* pCtx;
	WOLFSSL* pSSL;
};


#endif /* _TLSTRANSBLOCK_H_ */
