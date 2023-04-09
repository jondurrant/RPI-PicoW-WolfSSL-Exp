/*
 * Transport.h
 *
 *  Created on: 19 Mar 2023
 *      Author: jondurrant
 */

#ifndef _TRANSPORT_H_
#define _TRANSPORT_H_


extern "C" {
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>

#include "lwip/ip4_addr.h"
#include "lwip/ip_addr.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"

}

class Transport {
public:
	Transport();
	virtual ~Transport();

	/***
	 * Connect to remote TCP Socket
	 * @param host - Host address
	 * @param port - Port number
	 * @return true on success
	 */
	virtual bool transConnect(const char * host, uint16_t port)=0;

	/***
	 * Get status of the socket
	 * @return int <0 is error
	 */
	virtual int status() = 0;

	/***
	 * Close the socket
	 * @return true on success
	 */
	virtual bool transClose() = 0;


	/***
	 * Send bytes through socket
	 * @param pBuffer - Buffer to send from
	 * @param bytesToSend - number of bytes to send
	 * @return number of bytes sent
	 */
	virtual int32_t transSend(const void * pBuffer, size_t bytesToSend)=0;

	/***
	 * Send
	 * @param pBuffer - Buffer to read into
	 * @param bytesToRecv - max number of bytes to read
	 * @return bytes read, < 0 on error
	 */
	virtual int32_t transRead(void * pBuffer, size_t bytesToRecv) = 0;


	/***
	 * Print out buffer to stdio
	 * Print plain text and hex version of stream
	 * @param title - Title of the print
	 * @param pBuffer - buffer to print
	 * @param bytes - number of bytes in buffer
	 */
	void debugPrintBuffer(const char *title, const void * pBuffer, size_t bytes);

};

#endif /* _TRANSPORT_H_ */
