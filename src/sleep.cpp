#include "sleep.h"

#include "system.h"
#include "pcb.h"
#include "semaphor.h"
#include "kersem.h"
void SleepList::put(PCB *nit, Time tts){
	if(!prvi){
		prvi = new Elem(nit, tts, NULL, NULL);
		posl = prvi;
		rep=tts;
	}
	else{
		if(tts>=rep){
			posl->sled = new Elem(nit, tts-rep, posl, NULL);
			posl = posl->sled;
			rep = tts;
		}
		else{
			if(prvi->tts > tts){
				prvi->pret = new Elem(nit, tts, NULL, prvi);
				prvi->tts -= tts;
				prvi = prvi->pret;
			}
			else{
				int sum=prvi->tts;
				Elem *pom = prvi, *iza = NULL;
				while(tts >= sum){
					iza = pom;
					pom = pom->sled;
					sum += pom-> tts;
				}
				sum -= pom->tts;
				iza->sled = pom->pret = new Elem(nit, tts-sum, iza, pom);
				pom->tts = sum + pom->tts - tts;
			}
		}
	}



/*
	cout<<"Stavljena nit "<<nit->getId()<<endl;
	cout<<"Sleeplist: ";
	for(Elem *p=prvi;p!=posl;p=p->sled){
		lock
		cout<<p->nit->getId()<<"("<<p->tts<<"), ";
		unlock
	}

	cout<<posl->nit->getId()<<"("<<posl->tts<<")"<<endl;

*/



}

SleepList::~SleepList(){

	Elem *pom = prvi;
	while(pom){
		pom = pom->sled;
		delete(prvi);
		prvi = pom;
	}
}

void SleepList::update(){

	if(prvi && prvi->tts > 0) prvi->tts--;
	if(rep >0 ) rep--;
	if(!System::sem_flag){
		while(prvi && (prvi->tts == 0)){
			Elem *pom=prvi;
			if(pom->nit->stanje==BLOCKED){
				pom->nit->semreturn=0;
				pom->nit->stanje=READY;
				pom->nit->sem->remove(pom->nit->getId());
				pom->nit->sem->value++;
				Scheduler::put(pom->nit);
//				cout<<"odblokirana nit "<<pom->nit->getId()<<endl;
			}
			prvi = prvi->sled;
			prvi->pret = NULL;
			delete pom;
		}
		if(!prvi) {
			posl = NULL;
			rep = 0;
		}
	}

/*
	if(prvi)cout<<"Update: ";
	for(Elem *p=prvi; p && p!=posl; p=p->sled){
		cout<<p->nit->getId()<<"("<<p->tts<<")"<<", ";
	}
	if(posl)cout<<posl->nit->getId()<<"("<<posl->tts<<")"<<endl;
*/
}


int SleepList::wakeup(ID id){
	Elem *pom=prvi;
	while(pom && pom->nit->getId()!=id) pom=pom->sled;
	if(pom){
		if(prvi==pom) prvi = pom->sled;
		if(posl==pom) posl = pom->pret;
		if(pom->pret) pom->pret->sled=pom->sled;
		if(pom->sled) {
			pom->sled->tts+=pom->tts;
			pom->sled->pret=pom->pret;
		}
		pom->nit->semreturn=1;
		pom->nit->stanje=READY;
		Scheduler::put(pom->nit);
//		cout<<"odblokirana nit "<<pom->nit->getId()<<endl;
		delete pom;
		return 1;
	}
	return 0;


/*
	cout<<"Sleeplist: ";
	for(Elem *p=prvi; p && p!=posl; p=p->sled){
		cout<<p->nit->getId()<<"("<<p->tts<<")"<<", ";
	}
	if(posl)cout<<posl->nit->getId()<<endl;

*/



}

void SleepList::remove(ID id){
	Elem *pom=prvi;
	while(pom && pom->nit->getId()!=id) pom=pom->sled;
	if(pom){
		if(prvi==pom) prvi = pom->sled;
		if(posl==pom) posl = pom->pret;
		if(pom->pret) pom->pret->sled=pom->sled;
		if(pom->sled) {
			pom->sled->tts+=pom->tts;
			pom->sled->pret=pom->pret;
		}
		delete pom;
	}
}
