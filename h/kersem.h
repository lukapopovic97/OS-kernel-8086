#ifndef _KERSEM_H_
#define _KERSEM_H_

#include <iostream.h>
#include "thread.h"
class KernelSem{

private:
	friend class Semaphore;
	friend class SleepList;
	friend class System;
	friend class PCB;

	KernelSem(int init);
	~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n=0);

	int val();

	int value;

	struct Elem {
		Elem *sled;
		PCB* nit;
		int sleep;
		Elem(PCB *thread, int s) {
			nit = thread;
			sled=NULL;
			sleep=s;
		}
	};

	Elem *prvi, *posl;

	void put(PCB* nit, int s);
	Elem* get();
	void remove(ID id);
};

#endif
