
#include "list.h"
#include "system.h"

void List::put(PCB *nit)
{
	lock;
	if (!prvi) {
		prvi = new Elem(nit);
		posl = prvi;
	}
	else {
		posl->sled = new Elem(nit);
		posl = posl->sled;
	}
	unlock;
}

PCB * List::get()
{
	lock
	if (prvi) {
		PCB *pom = prvi->nit;
		Elem *del=prvi;
		prvi = prvi->sled;
		delete del;
		if (!prvi) posl = NULL;
		unlock
		return pom;
	}
	else {
		unlock
		return NULL;
	}
}

void List::remove(ID id){
	lock;
	Elem *pom=prvi;
	Elem *pret=NULL;
	while(pom && pom->nit->getId()!=id){
		pret=pom;
		pom=pom->sled;
	}
	if(pom){
		if(pom==prvi)prvi=prvi->sled;
		if(pom==posl)posl=pret;
		if(pret)pret->sled=pom->sled;
		delete(pom);
	}
	unlock;
}

Thread* List::get(ID id){
	lock;
	if(prvi==NULL){
		unlock;
		return NULL;
	}
	Elem *pom=prvi;
	while(pom && pom->nit->getId()!=id){
		pom=pom->sled;
	}
	if(pom==NULL){
		unlock;
		return NULL;
	}
	else {
		unlock;
		return pom->nit->myThread;
	}
}

List::~List()
{
	lock;
	Elem *pom = prvi;
	while (pom) {
		prvi = prvi->sled;
		delete(pom);
		pom = prvi;
	}
	unlock;
}

void List::stanje(){
	lock
	for(Elem *pom=prvi;pom!=posl;pom=pom->sled){
		cout<<pom->nit->getId()<<", ";
	}
	cout<<posl->nit->getId()<<endl;
	unlock
}

