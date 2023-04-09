/*
 * TestTrans.h
 *
 *  Created on: 4 Sept 2022
 *      Author: jondurrant
 */

#ifndef TEST_TESTTRANS_H_
#define TEST_TESTTRANS_H_

#include "pico/stdlib.h"
#include "FreeRTOS.h"
#include "task.h"

#include "Agent.h"

class TestTrans : public Agent{
public:
	TestTrans();
	virtual ~TestTrans();

protected:

	/***
	 * Run loop for the agent.
	 */
	virtual void run();


	/***
	 * Get the static depth required in words
	 * @return - words
	 */
	virtual configSTACK_DEPTH_TYPE getMaxStackSize();

	void test();

private:

	bool testConnect();
	bool testTrans();
	bool testTLS();

	int xTests = 0;
	int xSuccessful = 0;
};

#endif /* TEST_TESTTRANS_H_ */
