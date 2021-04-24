#include "kersem.h"
#include "semaphor.h"
#include "system.h"
#include <iostream.h>
#include "pcb.h"
KernelSem::KernelSem(int init){
	value = init;
	prvi=posl=NULL;
}

KernelSem::~KernelSem(){
	Elem *pom=prvi;
	while (pom) {
		if(pom->sleep) System::removeFromSleeplist(pom->nit->getId());
		pom->nit->stanje=READY;
		pom->nit->sem=NULL;
		pom->nit->semreturn=1;
		Scheduler::put(pom->nit);
		pom = pom->sled;
		delete prvi;
		prvi=pom;
	}
	prvi=posl=NULL;
}

int KernelSem::val(){
	return value;
}

int KernelSem::signal(int n){
	if(n<0) return n;

	if(value>=0){
		if(!n)value++;
		else value+=n;
		return 0;
	}
	System::sem_flag=1;
	Elem *pom;
	if(n==0){
		value++;
		pom=get();
		if(!pom){
			System::sem_flag=0;
			return 0;
		}
		if(pom->sleep) System::wakeup(pom->nit->getId());
		else {
			pom->nit->semreturn=1;
			pom->nit->stanje=READY;
			Scheduler::put(pom->nit);
//			cout<<"odblokirana nit "<<pom->nit->getId()<<endl;
		}
		delete pom;
		System::sem_flag=0;
		return 0;
	}
	else{
		int ret=0;
		value +=n;
		while(ret<n){
			pom=get();
			if(!pom)break;
			if(pom->sleep) ret+=System::wakeup(pom->nit->getId());
			else {
				pom->nit->semreturn=1;
				pom->nit->stanje=READY;
				Scheduler::put(pom->nit);
//				cout<<"odblokirana nit "<<pom->nit->getId()<<endl;
				ret++;
			}
			delete pom;
		}
		System::sem_flag=0;
		return ret;
	}
}

int KernelSem::wait(Time maxTimeToWait){
	lock
	value--;
//	cout<<"wait "<<Thread::getRunningId()<<endl;
//	cout<<"value = "<<value<<endl;
	if(value>=0){
		unlock
//		cout<<"nije blokirana"<<endl;
		return 1;
	}
	PCB::running->sem=this;
	if(maxTimeToWait){
		put((PCB*)PCB::running,1);


/*
			for(Elem *p=prvi; p && p!=posl; p=p->sled){
				cout<<p->nit->getId()<<", ";
			}
			if(posl)cout<<posl->nit->getId()<<endl;

*/


		unlock
		System::sleep(maxTimeToWait);
	}
	else {
		PCB::running->stanje = BLOCKED;
		put((PCB*)PCB::running, 0);


/*
		for(Elem *p=prvi; p && p!=posl; p=p->sled){
			cout<<p->nit->getId()<<", ";
		}
		if(posl)cout<<posl->nit->getId()<<endl;
*/


		unlock
		dispatch();
	}


	PCB::running->sem=NULL;

/*	cout<<"ret= "<< PCB::running->semreturn<<" id="<<Thread::getRunningId()<<endl;
	cout<<"value="<<value<<endl;
	for(Elem *p=prvi; p && p!=posl; p=p->sled){
		cout<<p->nit->getId()<<", ";
	}
	if(posl)cout<<posl->nit->getId()<<endl;
*/


	return PCB::running->semreturn;
}

void KernelSem::put(PCB *nit, int s){
	if (!prvi) {
		prvi = new Elem(nit, s);
		posl = prvi;
	}
	else {
		posl->sled = new Elem(nit, s);
		posl = posl->sled;
	}
}

KernelSem::Elem* KernelSem::get(){
	Elem *ret = prvi;
	if(prvi)prvi=prvi->sled;
	if(prvi==NULL)posl=NULL;
	return ret;
}

void KernelSem::remove(ID id){
	lock
	Elem *pom=prvi;
	Elem *pret=NULL;
	while(pom && pom->nit->getId()!=id){
		pret=pom;
		pom=pom->sled;
	}
	if(pom){
		if(pret)pret->sled=pom->sled;
		if(prvi==pom)prvi=pom->sled;
		if(posl==pom)posl=pret;
		delete pom;
	}
	unlock
}
