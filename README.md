# RPI-PicoW-WolfSSL-Exp

Example for using TLS Socket on the Rasperry PI Pico W. Running inside
FreeRTOS with a LWIP Socket which is then secured using WolfSSL.

Example does not use any predefined Keys or validate any certificates from the host.

Blog posts on this will be available on [DrJonEA.co.uk](https://drjonea.co.uk/IoT) and [YouTube @DrJonEA](https://youtube.com/@DrJonEA).

## Libraries

+ [FreeRTOS Kernel](https://www.freertos.org/)
+ [WolfSSL](https://www.wolfssl.com/)
+ [Pico SDK](https://github.com/raspberrypi/pico-sdk)
+ [LWIP - included in Pico SDK](https://savannah.nongnu.org/projects/lwip/)

## Examples: HTTPSGET
Example goes through a number of tests:
1. Connects to WIFI and checks connection
2. Performs HTTP Get against www.drjonea.co.uk. This will return a HTTP redirect to HTTPS site
3. Performs a HTTPS Get against www.drjonea.co.uk. This will return HTML, but large page so we truncate it

## Build
Environment variables are used to configure the WIFI SSID and Password. Please make sure that these are set:
+ WIFI_SSID - ID for your WIFI
+ WIFI_PASSWORD - Password for your WIFI

To build:
```
mkdir build
cd build
cmake ..
make
```
Flash to Pico W.

## STDIO
HTTPSGet/src/CMakeList.txt is defines to put STDIO on UART using GPIO 16 and 17. You may want to change this for your set up.