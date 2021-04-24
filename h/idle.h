/*
 * idle.h
 *
 *  Created on: Feb 21, 2019
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include "thread.h";


class Idle : public Thread{

friend class System;

private:
	Idle();
	void run();
	~Idle();
};



#endif /* IDLE_H_ */
