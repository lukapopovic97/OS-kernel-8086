#ifndef  _system_h_
#define _system_h_

#include "SCHEDULE.H"
#include "thread.h"
#include <dos.h>
#include "idle.h"
#include "sleep.h"

#define lock System::system_lock();
#define unlock System::system_unlock();

typedef void interrupt (*pInterrupt)(...);

class System {

public:
	static void inic();
	static void restore();
	
	static void dispatch();
	static void sleep(Time timeToSleep);
	static void removeFromSleeplist(ID id);
	static int wakeup(ID id);
	static void system_lock();
	static void system_unlock();
	static volatile int context_switch;

	static void interrupt timer(...);

	static volatile Time cnt;
	static volatile unsigned flag;
	static volatile unsigned timer_flag;
	static volatile unsigned sem_flag;
	static SleepList* sleeping;

	static pInterrupt oldISR;

	static PCB* main;
	static Idle* idle;
	

};

#endif // ! _system_h_
