#ifndef SLEEP_H_
#define SLEEP_H_

#include <iostream.h>

class PCB;
#include "thread.h"
class SleepList{
private:
	struct Elem{
		PCB *nit;
		Elem *pret, *sled;
		volatile Time tts;
		Elem(PCB *thread, Time t, Elem *p, Elem *s){
			nit = thread;
			tts = t;
			pret = p;
			sled = s;
		}
	};

	Elem *prvi, *posl;
	int rep;

public:
	void put(PCB *nit, Time tts);
	SleepList():prvi(NULL), posl(NULL), rep(0){}
	~SleepList();
	void update();
	int wakeup(ID id);
	void remove(ID id);
};





#endif /* SLEEP_H_ */
