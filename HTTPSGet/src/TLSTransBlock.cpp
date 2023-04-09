/*
 * TLSTransBlock.cpp
 *
 *  Created on: 3 Apr 2023
 *      Author: jondurrant
 */

#include "TLSTransBlock.h"
#include <stdlib.h>
#include "pico/stdlib.h"
#include <errno.h>

#include <stdio.h>

TLSTransBlock::TLSTransBlock() {
	xHostDNSFound = xSemaphoreCreateBinary(  );

	wolfSSL_Init();/* Initialize wolfSSL */
	//wolfSSL_Debugging_ON();



	/* Create the WOLFSSL_CTX */
	pCtx = wolfSSL_CTX_new(wolfTLSv1_2_client_method());
	if ( pCtx == NULL){
		printf("wolfSSL_CTX_new error.\n");
	}

	wolfSSL_SetIORecv(pCtx, TLSTransBlock::IORecv);
	wolfSSL_SetIOSend(pCtx, TLSTransBlock::IOSend);
	wolfSSL_CTX_set_verify(pCtx, SSL_VERIFY_NONE, NULL);

}

TLSTransBlock::~TLSTransBlock() {
	// NOP
}

/***
 * Send bytes through socket
 * @param pBuffer - Buffer to send from
 * @param bytesToSend - number of bytes to send
 * @return number of bytes sent
 */
int32_t TLSTransBlock::transSend( const void * pBuffer, size_t bytesToSend){
	uint32_t dataOut;

	//debugPrintBuffer("TLSTransBlock::transSend", pBuffer, bytesToSend);

	//dataOut = write(xSock,(uint8_t *)pBuffer, bytesToSend);
	dataOut = wolfSSL_write(pSSL, (uint8_t *)pBuffer, bytesToSend);
	if (dataOut != bytesToSend){
		printf("Send failed %d\n", dataOut);
	}
	return dataOut;
}


/***
 * Send
 * @param pBuffer
 * @param bytesToRecv
 * @return
 */
int32_t TLSTransBlock::transRead( void * pBuffer, size_t bytesToRecv){
	int32_t dataIn=0;
	uint32_t bytesAv=0;
	int ret;
	int buffered;


	//Do non blocking check on a 1 byte read
	if (bytesToRecv == 1){
		//Only check if socket is OK
		if (status() >= 0){

			ret = ioctl (xSock,FIONREAD,&bytesAv);
			buffered = wolfSSL_pending(pSSL);

			if ((bytesAv ==0) && (buffered == 0)){
				return 0;
			}
		}
	}

	dataIn = wolfSSL_read(pSSL, (uint8_t *)pBuffer, bytesToRecv);

	return dataIn;
}





/***
 * Connect to remote TLS Socket
 * @param host - Host address
 * @param port - Port number
 * @return true on success
 */
bool TLSTransBlock::transConnect(const char * host, uint16_t port){
	err_t res = dns_gethostbyname(host, &xHost, TLSTransBlock::dnsCB, this);

	strcpy(xHostName, host);
	xPort = port;

	if (xSemaphoreTake(xHostDNSFound, TLS_TRANSPORT_WAIT) != pdTRUE){
		printf("DNS Timeout on Connect: %s, %d", host, res);
		//return false;
	}

	return transConnect();
}


/***
 * Connect to socket previously stored ip address and port number
 * @return true if socket openned
 */
bool TLSTransBlock::transConnect(){
	struct sockaddr_in serv_addr;
	int                ret, err;


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

	/* Create WOLFSSL object */
	if( (pSSL = wolfSSL_new(pCtx)) == NULL) {
	    printf("wolfSSL_new error.\n");
	    return false;
	}

	if ((ret = wolfSSL_set_fd(pSSL, xSock)) != WOLFSSL_SUCCESS){
		printf("Failed to set the FD");
		return false;
	}



	ret = wolfSSL_connect(pSSL);
	err = wolfSSL_get_error(pSSL, ret);

    if (ret != WOLFSSL_SUCCESS){
        printf("err %d: failed to connect to wolfSSL %d\n", err, ret);
        return false;
    }

	//printf("Connect success\n");
	return true;
}


/***
 * Get status of the socket
 * @return int <0 is error
 */
int TLSTransBlock::status(){
	int error = 0;
	socklen_t len = sizeof (error);
	int retval = getsockopt (xSock, SOL_SOCKET, SO_ERROR, &error, &len);
	return error;
}

/***
 * Close the socket
 * @return true on success
 */
bool TLSTransBlock::transClose(){
	wolfSSL_free(pSSL);
	wolfSSL_CTX_free(pCtx);
	wolfSSL_Cleanup();
	closesocket(xSock);
	return true;
}

/***
 * Call back function for the DNS lookup
 * @param name - server name
 * @param ipaddr - resulting IP address
 * @param callback_arg - poiter to TLSTransBlock object
 */
void TLSTransBlock::dnsCB(const char *name, const ip_addr_t *ipaddr, void *callback_arg){
	TLSTransBlock *self = (TLSTransBlock *) callback_arg;
	self->dnsFound(name, ipaddr, callback_arg);
}


/***
 * Called when DNS is returned
 * @param name - server name
 * @param ipaddr - ip address of server
 * @param callback_arg - this TLSTransBlock object
 */
void TLSTransBlock::dnsFound(const char *name, const ip_addr_t *ipaddr, void *callback_arg){
	memcpy(&xHost, ipaddr, sizeof(xHost));

	printf("DNS Found %s copied to xHost %s\n", ipaddr_ntoa(ipaddr), ipaddr_ntoa(&xHost));
	xSemaphoreGiveFromISR(xHostDNSFound, NULL );
}





int TLSTransBlock::IOSend(WOLFSSL* ssl, char* buff, int sz, void* ctx){
    /* By default, ctx will be a pointer to the file descriptor to write to.
     * This can be changed by calling wolfSSL_SetIOWriteCtx(). */
    int sockfd = *(int*)ctx;
    int sent;

    //printf("Sending %d", sz);

    /* Receive message from socket */
    if ((sent = send(sockfd, buff, sz, 0)) == -1) {
        /* error encountered. Be responsible and report it in wolfSSL terms */

    	printf("Send Error %d %d", sent, errno);

        int err = wolfSSL_get_error(ssl, errno);
        char errorString[80];
        wolfSSL_ERR_error_string(err, errorString);
    	printf(errorString);

        switch (errno) {
        #if EAGAIN != EWOULDBLOCK
        case EAGAIN: /* EAGAIN == EWOULDBLOCK on some systems, but not others */
        #endif
        case EWOULDBLOCK:
            printf( "would block\n");
            return WOLFSSL_CBIO_ERR_WANT_WRITE;
        case ECONNRESET:
            printf("connection reset\n");
            return WOLFSSL_CBIO_ERR_CONN_RST;
        case EINTR:
            printf("socket interrupted\n");
            return WOLFSSL_CBIO_ERR_ISR;
        case EPIPE:
            printf("socket EPIPE\n");
            return WOLFSSL_CBIO_ERR_CONN_CLOSE;
        case EINPROGRESS:
        	printf("socket EINPROGRESS\n");
			return 0;
        default:
            printf("general error\n");
            return WOLFSSL_CBIO_ERR_GENERAL;
        }

    }


    /* successful send */
    //printf("my_IOSend: sent %d bytes to %d\n", sz, sockfd);
    return sent;
}

int TLSTransBlock::IORecv(WOLFSSL* ssl, char* buff, int sz, void* ctx){
    /* By default, ctx will be a pointer to the file descriptor to read from.
     * This can be changed by calling wolfSSL_SetIOReadCtx(). */
    int sockfd = *(int*)ctx;
    int recvd;


    /* Receive message from socket */
    if ((recvd = recv(sockfd, buff, sz, 0)) == -1) {
        /* error encountered. Be responsible and report it in wolfSSL terms */

        int err = wolfSSL_get_error(ssl, errno);
        printf("IO RECEIVE ERROR: errno=%d sslErr=%d", errno, err);
        switch (errno) {
        #if EAGAIN != EWOULDBLOCK
        case EAGAIN: /* EAGAIN == EWOULDBLOCK on some systems, but not others */
        #endif
        case EWOULDBLOCK:
            if (!wolfSSL_dtls(ssl) || wolfSSL_get_using_nonblock(ssl)) {
                printf("would block\n");
                return WOLFSSL_CBIO_ERR_WANT_READ;
            }
            else {
                printf("socket timeout\n");
                return WOLFSSL_CBIO_ERR_TIMEOUT;
            }
        case ECONNRESET:
            printf("connection reset\n");
            return WOLFSSL_CBIO_ERR_CONN_RST;
        case EINTR:
            printf("socket interrupted\n");
            return WOLFSSL_CBIO_ERR_ISR;
        case ECONNREFUSED:
            printf("connection refused\n");
            return WOLFSSL_CBIO_ERR_WANT_READ;
        case ECONNABORTED:
            printf("connection aborted\n");
            return WOLFSSL_CBIO_ERR_CONN_CLOSE;
        case EINPROGRESS:
        	printf("socket EINPROGRESS\n");
			return 0;

        default:
            printf("general error\n");
            return WOLFSSL_CBIO_ERR_GENERAL;
        }


    }
    else if (recvd == 0) {
    	int error = 0;
		socklen_t len = sizeof (error);
		int retval = getsockopt (sockfd, SOL_SOCKET, SO_ERROR, &error, &len);
        printf("Connection closed. Status %d\n", error);
        return WOLFSSL_CBIO_ERR_CONN_CLOSE;
    }


    /* successful receive */
    //printf("Received %d bytes from %d\n", sz, sockfd);
    return recvd;
}


