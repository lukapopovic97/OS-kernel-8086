#ifndef _ivtentry_h_
#define _ivtentry_h_

#include "event.h"

typedef void interrupt (*pInterrupt)(...);

#define PREPAREENTRY(ivtNo,flag)\
void interrupt inter##ivtNo(...);\
IVTEntry entry##ivtNo(ivtNo, inter##ivtNo);\
void interrupt inter##ivtNo(...) {\
	entry##ivtNo.signal();\
	if(##flag)\
	entry##ivtNo.callOld();\
}

class IVTEntry{

friend class KernelEv;

public:
	IVTEntry(IVTNo no, pInterrupt newIntr);
	~IVTEntry();
	void signal();
	void callOld();

private:
	pInterrupt oldInterrupt;
	KernelEv* event;
	IVTNo ivtNo;
	static IVTEntry* ivtable[256];
	static void set(IVTNo no, KernelEv* ev);
	static void remove(IVTNo no, KernelEv* ev);

};


#endif
