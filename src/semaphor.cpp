#include "semaphor.h"
#include "system.h"
#include "kersem.h"

Semaphore::Semaphore(int init){
	lock
	myImpl= new KernelSem(init);
	unlock
}

Semaphore::~Semaphore(){
	lock
	delete myImpl;
	unlock
}

int Semaphore::wait(Time maxTimeToWait){

	return myImpl->wait(maxTimeToWait);
}

int Semaphore::signal(int n){
	lock
	int ret = myImpl->signal(n);
	unlock
	return ret;
}

int Semaphore::val() const{
	lock
	int ret = myImpl->val();
	unlock
	return ret;
}
