#include "thread.h"
#include "pcb.h"
#include "system.h"
#include <iostream.h>

void Thread::start()
{
	lock
	if (myPCB->stanje == NEW) {
		myPCB->createStack();
		myPCB->stanje = READY;
		if(myPCB->myThread!=(Thread*)System::idle)Scheduler::put(myPCB);
		//cout<<"Dodato u Scheduler "<<getId()<<endl;
	}
	unlock
}

void Thread::waitToComplete()
{
	lock
	/*int run_mypcb=PCB::running!=myPCB;
	int mypcb_main=myPCB!=System::main;
	int mythread_idle=myPCB->myThread!=(Thread*) System::idle;
	cout<<"PCB::running!=myPCB "<<run_mypcb<<" myPCB!=System::main "<<mypcb_main<<endl;
	cout<<"myPCB->myThread!=(Thread*) System::idle "<<mythread_idle<<endl;*/
	if (PCB::running!=myPCB && myPCB!=System::main && myPCB->myThread!=(Thread*) System::idle) myPCB->waitToComplete();
	unlock;
	dispatch();
}

Thread::~Thread()
{
	waitToComplete();
	delete (myPCB);
}

Thread::Thread(StackSize stackSize, Time timeSlice)
{
	myPCB = new PCB(this, stackSize, timeSlice);
}

void dispatch() {
	lock
	System::dispatch();
	unlock
}

ID Thread::getId(){
	return myPCB->getId();
}

ID Thread::getRunningId(){
	return PCB::getRunningId();
}
Thread * Thread::getThreadById(ID id){
	return PCB::getThreadById(id);
}

