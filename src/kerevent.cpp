#include "kerevent.h"
#include "ivtentry.h"
#include "system.h"
#include "pcb.h"

KernelEv::KernelEv(IVTNo ivtNo){
	this->ivtNo=ivtNo;
	myThread=(PCB*)PCB::running;
	waiting=0;
	IVTEntry::set(ivtNo, this);
}

KernelEv::~KernelEv(){
	if(waiting){
		myThread->stanje=READY;
		Scheduler::put(myThread);
		IVTEntry::remove(ivtNo, this);
	}
}

void KernelEv::wait(){
	lock
	if(PCB::running==myThread){
		waiting=1;
		myThread->stanje=BLOCKED;
		unlock
		dispatch();
	}
	else unlock
}

void KernelEv::signal(){
	lock
	if(waiting){
		myThread->stanje=READY;
		waiting=0;
		Scheduler::put(myThread);
		unlock
		dispatch();
	}
	else unlock
}
