/*
 * WifiHelper.cpp
 *
 *  Created on: 24 Nov 2021
 *      Author: jondurrant
 */

#include "WifiHelper.h"

#include "pico/cyw43_arch.h"
#include "pico/util/datetime.h"

#include "FreeRTOS.h"
#include "task.h"


WifiHelper::WifiHelper() {
	// NOP

}

WifiHelper::~WifiHelper() {
	// NOP
}

/***
 * Initialise the controller
 * @return true if successful
 */
bool WifiHelper::init(){
	// Init Wifi Adapter
	int res = cyw43_arch_init();
	if (res) {
		return false;
	}

	//cyw43_wifi_pm(&cyw43_state ,CYW43_AGGRESSIVE_PM);
	cyw43_wifi_pm(&cyw43_state ,CYW43_PERFORMANCE_PM);

	return true;

}

bool WifiHelper::join(const char *sid, const char *password,  uint8_t retries){
	cyw43_arch_enable_sta_mode();
	printf("Connecting to WiFi... %s \n", WIFI_SSID);

	//Loop trying to connect to Wifi
	int r =-1;
	uint8_t attempts = 0;
	while (r < 0){
		attempts++;
		r = cyw43_arch_wifi_connect_timeout_ms(sid, password, CYW43_AUTH_WPA2_AES_PSK, 60000);

		if (r){
			printf("Failed to join AP.\n");
			if (attempts >= retries){
				return false;
			}
			vTaskDelay(2000);
		}
	}
	return true;

}



/***
 * Get IP address of unit
 * @param ip - output uint8_t[4]
 */
bool WifiHelper::getIPAddress(uint8_t *ip) {
	memcpy(ip, netif_ip4_addr(&cyw43_state.netif[0]), 4);
	return true;
}

/***
 * Get IP address of unit
 * @param ips - output char * up to 16 chars
 * @return - true if IP addres assigned
 */
bool WifiHelper::getIPAddressStr(char *ips) {
	char *s = ipaddr_ntoa(netif_ip4_addr(&cyw43_state.netif[0]));
	strcpy(ips, s);
	return true;
}

/***
 * Get Gateway address
 * @param ip - output uint8_t[4]
 */
bool WifiHelper::getGWAddress(uint8_t *ip) {
	memcpy(ip, netif_ip4_gw(&cyw43_state.netif[0]), 4);
	return true;
}

/***
 * Get Gateway address
 * @param ips - output char * up to 16 chars
 * @return - true if IP addres assigned
 */
bool WifiHelper::getGWAddressStr(char *ips) {
	char *s = ipaddr_ntoa(netif_ip4_gw(&cyw43_state.netif[0]));
	strcpy(ips, s);
	return true;
}

/***
 * Get Net Mask address
 * @param ip - output uint8_t[4]
 */
bool WifiHelper::getNetMask(uint8_t *ip) {
	memcpy(ip, netif_ip4_netmask(&cyw43_state.netif[0]), 4);
	return true;
}

/***
 * Get Net Mask
 * @param ips - output char * up to 16 chars
 * @return - true if IP addres assigned
 */
bool WifiHelper::getNetMaskStr(char *ips) {
	char *s = ipaddr_ntoa(netif_ip4_netmask(&cyw43_state.netif[0]));
	strcpy(ips, s);
	return true;
}

/***
 * Get the mac address as a string
 * @param macStr: pointer to string of at least 14 characters
 * @return true if successful
 */
bool WifiHelper::getMACAddressStr(char *macStr) {
	uint8_t mac[6];
	int r =  cyw43_wifi_get_mac(&cyw43_state, CYW43_ITF_STA,  mac);

	if (r == 0) {
		for (uint8_t i = 0; i < 6; i++) {
			if (mac[i] < 16){
				sprintf(&macStr[i*2], "0%X", mac[i]);
			} else {
				sprintf(&macStr[i*2], "%X", mac[i]);
			}
		}
		macStr[13] = 0;
		return true;
	}
	return false;
}


/***
 * Returns if joined to the network and we have a link
 * @return true if joined.
 */
bool WifiHelper::isJoined(){
	int res = cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA);
	return (res >= 0);
}


