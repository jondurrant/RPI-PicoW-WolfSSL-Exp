/***
 * main.cpp - HTTP Get over socket
 * Jon Durrant
 * 4-Oct-2022
 *
 *
 */

#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include <stdio.h>

#include "lwip/ip4_addr.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>


#include "WifiHelper.h"
#include "TestTrans.h"



//Check these definitions where added from the makefile
#ifndef WIFI_SSID
#error "WIFI_SSID not defined"
#endif
#ifndef WIFI_PASSWORD
#error "WIFI_PASSWORD not defined"
#endif

// LED PAD defintions
#define PULSE_LED       0


#define TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )


void runTimeStats(){
  TaskStatus_t         * pxTaskStatusArray;
  volatile UBaseType_t uxArraySize, x;
  unsigned long        ulTotalRunTime;


  /* Take a snapshot of the number of tasks in case it changes while this
  function is executing. */
  uxArraySize = uxTaskGetNumberOfTasks();
  printf("Number of tasks %d\n", uxArraySize);

  /* Allocate a TaskStatus_t structure for each task.  An array could be
  allocated statically at compile time. */
  pxTaskStatusArray = (TaskStatus_t*) pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));

  if (pxTaskStatusArray != NULL){
    /* Generate raw status information about each task. */
    uxArraySize = uxTaskGetSystemState(pxTaskStatusArray,
                                       uxArraySize,
                                       &ulTotalRunTime);



    /* For each populated position in the pxTaskStatusArray array,
    format the raw data as human readable ASCII data. */
    for (x = 0; x < uxArraySize; x++){
      printf("Task: %d \t cPri:%d \t bPri:%d \t hw:%d \t%s\n",
             pxTaskStatusArray[x].xTaskNumber,
             pxTaskStatusArray[x].uxCurrentPriority,
             pxTaskStatusArray[x].uxBasePriority,
             pxTaskStatusArray[x].usStackHighWaterMark,
             pxTaskStatusArray[x].pcTaskName
      );
    }


    /* The array is no longer needed, free the memory it consumes. */
    vPortFree(pxTaskStatusArray);
  } else{
    printf("Failed to allocate space for stats\n");
  }

  HeapStats_t heapStats;
  vPortGetHeapStats(&heapStats);
  printf("HEAP avl: %d, blocks %d, alloc: %d, free: %d\n",
         heapStats.xAvailableHeapSpaceInBytes,
         heapStats.xNumberOfFreeBlocks,
         heapStats.xNumberOfSuccessfulAllocations,
         heapStats.xNumberOfSuccessfulFrees
  );

}


void main_task(void* params){

  printf("Main task started\n");

  if (WifiHelper::init()){
    printf("Wifi Controller Initialised\n");
  } else {
    printf("Failed to initialise controller\n");
    return;
  }


  printf("Connecting to WiFi... %s \n", WIFI_SSID);

  if (WifiHelper::join(WIFI_SSID, WIFI_PASSWORD)){
    printf("Connect to Wifi\n");
  }
  else {
    printf("Failed to connect to Wifi \n");
  }


  //Print MAC Address
  char macStr[20];
  WifiHelper::getMACAddressStr(macStr);
  printf("MAC ADDRESS: %s\n", macStr);

  //Print IP Address
  char ipStr[20];
  WifiHelper::getIPAddressStr(ipStr);
  printf("IP ADDRESS: %s\n", ipStr);


  TestTrans testTrans;
  testTrans.start("test", TASK_PRIORITY);

  while (true){

    runTimeStats();

    vTaskDelay(3000);

    if (!WifiHelper::isJoined()){
      printf("AP Link is down\n");

      if (WifiHelper::join(WIFI_SSID, WIFI_PASSWORD)){
        printf("Connect to Wifi\n");
      } else {
        printf("Failed to connect to Wifi \n");
      }
    }


  }

}


void vLaunch(void) {
  TaskHandle_t task;

  xTaskCreate(main_task, "MainThread", 2048, NULL, TASK_PRIORITY, &task);

  /* Start the tasks and timer running. */
  vTaskStartScheduler();
}


int main(void) {
  stdio_init_all();
  sleep_ms(2000);
  printf("GO\n");

  /* Configure the hardware ready to run the demo. */
  const char* rtos_name;
  rtos_name = "FreeRTOS";
  printf("Starting %s on core 0:\n", rtos_name);
  vLaunch();

  return 0;
}
