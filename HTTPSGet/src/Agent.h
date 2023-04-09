/*
 * Agent.h
 *
 * Abstract agent interface to an active agent object that runs as
 * FreeRTOS task
 *
 *  Created on: 15 Aug 2022
 *      Author: jondurrant
 */

#ifndef SRC_AGENT_H_
#define SRC_AGENT_H_

#define MAX_NAME_LEN 20

#include "FreeRTOS.h"
#include "task.h"


class Agent {
public:
	/***
	 * Constructor
	 */
	Agent();

	/***
	 * Destructor
	 */
	virtual ~Agent();

	/***
	 * Start the task
	 * @param name - Give the task a name (<20 characters)
	 * @param priority - priority - 0 is idle
	 * @return
	 */
	virtual  bool start(const char *name, UBaseType_t priority = tskIDLE_PRIORITY);

	/***
	 * Stop task
	 * @return
	 */
	virtual void stop();


	/***
	 * Get high water for stack
	 * @return close to zero means overflow risk
	 */
	virtual unsigned int getStakHighWater();

	/***
	 * Get the FreeRTOS task being used
	 * @return
	 */
	virtual TaskHandle_t getTask();

protected:
	/***
	 * Start the task via static function
	 * @param pvParameters - will be the Agent object
	 */
	static void vTask( void * pvParameters );

	/***
	 * Task main run loop
	 */
	virtual void run()=0;

	/***
	 * Get the static depth required in words
	 * @return - words
	 */
	virtual configSTACK_DEPTH_TYPE getMaxStackSize()=0;

	//The task
	TaskHandle_t xHandle = NULL;

	char pName[MAX_NAME_LEN];


};


#endif /* SRC_AGENT_H_ */
