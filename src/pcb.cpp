#include "pcb.h"
#include "SCHEDULE.H"
#include "system.h"
#include <iostream.h>

PCB::PCB(Thread* myThread, StackSize stackSize, Time timeSlice) {
	lock
	this->myThread = myThread;
	this->stackSize = stackSize>65535 ? 65535 : stackSize;
	this->timeSlice = timeSlice;
	stack = NULL;
	sp = ss = bp = NULL;
	stanje = NEW;
	waiting = new List();
	id = nextID++;
	semreturn=0;
	sem=NULL;
	PCB::sveniti->put(this);
//	cout<<"Stanje liste: ";
//	PCB::sveniti->stanje();
	unlock
	//cout<<"Kreirana nit id stackSize timeSlice stanje"<<this->id<<"	"<<this->stackSize<<" "<<this->timeSlice<<" "<<this->stanje<<endl;
}

void PCB::waitToComplete()
{
	/*int runstanje_new=running->stanje != NEW;
	int runstanje_end=running->stanje!= ENDED;
	int sta_end=stanje!=ENDED;
	int sta_new=stanje!= NEW;
	lock
	cout<<"running->stanje!=NEW "<<runstanje_new<<" running->stanje!= ENDED "<<runstanje_end<<endl;
	cout<<" stanje!=ENDED "<<sta_end<<" stanje!=NEW "<<sta_new<<endl;
	unlock*/
	if (running->stanje != NEW && running->stanje!= ENDED && stanje!=ENDED && stanje != NEW) {
		waiting->put((PCB*)running);
		running->stanje = BLOCKED;
	}
}

void PCB::wrapper()
{
	//cout<<"U wrapperu"<<endl;
	running->myThread->run();
	lock
	PCB *pom = running->waiting->get();
	while (pom) {
		pom->stanje = READY;
		Scheduler::put(pom);
//		cout<<"Back to scheduler"<<endl;
		pom = running->waiting->get();
	}
	running->stanje = ENDED;
	unlock
	dispatch();
}

void PCB::createStack()
{
	StackSize ssize = (stackSize / sizeof(unsigned)) * sizeof(unsigned char);
	this->stack = new unsigned[ssize];
	//cout<<"Velicina steka "<<ssize<<endl;
	stack[ssize - 1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[ssize - 2] = FP_SEG(wrapper);
	stack[ssize - 3] = FP_OFF(wrapper);
	//cout<<"wrapper"<<endl;
	sp = FP_OFF(stack + (ssize - 12));
	ss = FP_SEG(stack + (ssize - 12));
	bp = FP_OFF(stack + (ssize - 12));
#endif
}


PCB::~PCB() {
	lock
	PCB::sveniti->remove(id);
//	cout<<"Stanje liste: ";
//	PCB::sveniti->stanje();
	delete waiting;
	delete stack;
	unlock
}

ID PCB::getId(){
	return id;
}


ID PCB::getRunningId(){
	lock;
	if(PCB::running!=NULL){
		unlock;
		return PCB::running->id;
	}
	else {
		unlock;
		return -1;
	}
}


Thread * PCB::getThreadById(ID id){
	return PCB::sveniti->get(id);
}

int PCB::nextID = 0;
volatile PCB* PCB::running = NULL;
List* PCB::sveniti=new List();
