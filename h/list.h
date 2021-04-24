#ifndef _list_h_
#define _list_h_

#include <iostream.h>
#include "pcb.h";

class List {

private:
	struct Elem {
		Elem* sled;
		PCB* nit;
		Elem(PCB *thread) {
			nit = thread;
			sled=NULL;
		}
	};

	Elem *prvi, *posl;
public:

	void put(PCB *nit);
	PCB* get();
	void remove(ID id);
	Thread* get(ID id);
	List() :prvi(NULL), posl(NULL) {}
	~List();

	void stanje();
};

#endif // !_list_h_
