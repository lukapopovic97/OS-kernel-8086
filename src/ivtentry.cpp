// File: ivtentry.cpp

#include "ivtentry.h"
#include <dos.h>
#include <iostream.h>
#include "system.h"
#include "kerevent.h"

IVTEntry* IVTEntry::ivtable[256];


IVTEntry::IVTEntry(IVTNo no, pInterrupt newIntr){
	lock
	ivtNo=no;
#ifndef BCC_BLOCK_IGNORE
	oldInterrupt=getvect(no);
	setvect(no, newIntr);
#endif
	ivtable[no]=this;
	event=NULL;
	unlock
}

IVTEntry::~IVTEntry(){
	lock
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, oldInterrupt);
#endif
	unlock
}

void IVTEntry::signal(){
	if(event!=NULL) event->signal();
}

void IVTEntry::callOld(){
	oldInterrupt();
}

void IVTEntry::set(IVTNo no, KernelEv* ev){
	lock
	if(!(ivtable[no]->event)) ivtable[no]->event=ev;
	unlock
}

void IVTEntry::remove(IVTNo no, KernelEv* ev){
	lock
	if(ivtable[no]->event==ev)ivtable[no]->event==NULL;
	unlock
}
