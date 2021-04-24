#ifndef _pcb_h_
#define _pcb_h_

#include "thread.h"
#include <stdlib.h>


#include "list.h"
class List;
class KernelSem;
typedef enum {NEW, READY, BLOCKED, ENDED} Stanje;

class PCB {

private:
	friend class Thread;
	friend class System;
	friend class List;
	friend class SleepList;
	friend class KernelSem;
	friend class KernelEv;

	PCB (Thread* myThread, StackSize stackSize, Time timeSlice);
	~PCB();

	void waitToComplete();
	static void wrapper();
	void createStack();
	ID getId();
	static ID getRunningId();
	static Thread * getThreadById(ID id);

	StackSize stackSize;
	Time timeSlice;

	Thread* myThread;
	static volatile PCB* running;
	static ID nextID;
	static List* sveniti;

	unsigned* stack;
	unsigned sp, bp, ss;
	ID id;

	List* waiting;
	volatile Stanje stanje;

	int semreturn;
	KernelSem *sem;
};

#endif // !_pcb_h_

