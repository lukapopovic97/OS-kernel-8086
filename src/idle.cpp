/*
 * idle.cpp
 *
 *  Created on: Feb 21, 2019
 *      Author: OS1
 */
#include "idle.h";
#include<iostream.h>
#include "system.h"

void Idle::run(){
	while(1){
/*		lock
		cout<<"-------idle------"<<endl;
		unlock
*/		for (int j = 0; j< 10000; ++j)
			for (int k = 0; k < 30000; ++k);
	}
}

Idle::Idle():Thread(32000, defaultTimeSlice){}

Idle::~Idle(){};
