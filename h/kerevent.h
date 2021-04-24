#ifndef _kerevent_h_
#define _kerevent_h_

#include "event.h"
class PCB;

class KernelEv {

private:
	friend class Event;
	friend class IVTEntry;

	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();

	IVTNo ivtNo;
	PCB* myThread;
	unsigned waiting;
};
#endif
