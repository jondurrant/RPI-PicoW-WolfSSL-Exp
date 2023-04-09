/*
 * WifiHelper.h
 *
 *  Created on: 24 Nov 2021
 *      Author: jondurrant
 */

#ifndef SRC_WIFIHELPER_H_
#define SRC_WIFIHELPER_H_

#include <stdlib.h>
#include "pico/stdlib.h"

#ifndef WIFI_RETRIES
#define WIFI_RETRIES 3
#endif


class WifiHelper {
public:
	WifiHelper();

	virtual ~WifiHelper();

	/***
	 * Initialise the controller
	 * @return true if successful
	 */
	static bool init();

	/***
	 * Get IP address of unit
	 * @param ip - output uint8_t[4]
	 * @return - true if IP addres assigned
	 */
	static bool getIPAddress(uint8_t *ip);

	/***
	 * Get IP address of unit
	 * @param ips - output char * up to 16 chars
	 * @return - true if IP addres assigned
	 */
	static bool getIPAddressStr(char *ips);

	/***
	 * Get Gateway address
	 * @param ip - output uint8_t[4]
	 */
	static bool getGWAddress(uint8_t *ip);

	/***
	 * Get Gateway address
	 * @param ips - output char * up to 16 chars
	 * @return - true if IP addres assigned
	 */
	static bool getGWAddressStr(char *ips) ;

	/***
	 * Get Net Mask address
	 * @param ip - output uint8_t[4]
	 */
	static bool getNetMask(uint8_t *ip) ;

	/***
	 * Get Net Mask
	 * @param ips - output char * up to 16 chars
	 * @return - true if IP addres assigned
	 */
	static bool getNetMaskStr(char *ips) ;

	/***
	 * Get the mac address as a string
	 * @param macStr: pointer to string of at least 14 characters
	 * @return true if successful
	 */
	static bool getMACAddressStr(char *macStr);

	/***
	 *  Join a Wifi Network
	 * @param sid - string of the SID
	 * @param password - Password for network
	 * @param retries - Number of times to retry, defalts to 3.
	 * @return true if successful
	 */
	static bool join(const char *sid, const char *password, uint8_t retries = WIFI_RETRIES);

	/***
	 * Returns if joined to the network and we have a link
	 * @return true if joined.
	 */
	static bool isJoined();

private:


};

#endif /* SRC_WIFIHELPER_H_ */

