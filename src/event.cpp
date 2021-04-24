#include "event.h"
#include "system.h"
#include "kerevent.h"

Event::Event(IVTNo ivtNo){
	lock
	myImpl=new KernelEv(ivtNo);
	unlock
}

Event::~Event(){
	lock
	delete myImpl;
	unlock
}

void Event::wait(){
	myImpl->wait();
}

void Event::signal(){
	myImpl->signal();
}
