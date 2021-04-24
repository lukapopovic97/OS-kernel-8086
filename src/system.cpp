#include "system.h"
#include "thread.h"
#include "pcb.h"
#include <dos.h>
#include <iostream.h>

void System::inic()
{
	//cout<<"U inic()"<<endl;
#ifndef BCC_BLOCK_IGNORE
	asm cli
#endif;
	PCB::running = new PCB(NULL, defaultStackSize, defaultTimeSlice);
	cnt=PCB::running->timeSlice;
	main = (PCB*)PCB::running;
	main->stanje = READY;

	idle=new Idle();
	idle->start();

	sleeping=new SleepList();

#ifndef BCC_BLOCK_IGNORE
	oldISR = getvect(0x08);
	setvect(0x08, timer);
	setvect(0x60, oldISR);
	asm sti
	//cout<<"inic kraj"<<endl;
#endif
}

void System::restore()
{

#ifndef BCC_BLOCK_IGNORE
	asm	cli
	setvect(0x08, oldISR);
#endif

	delete main;
	main = NULL;

	delete idle;
	idle=NULL;

	delete sleeping;
	sleeping=NULL;

	delete PCB::sveniti;
	PCB::sveniti=NULL;

	PCB::running = NULL;
#ifndef BCC_BLOCK_IGNORE
	asm	sti
#endif
}

void interrupt System::timer(...) {

	//tick();

	// poziv stare prekidne rutine koja se
	// nalazila na 08h, a sad je na 60h
	// poziva se samo kada nije zahtevana promena
	// konteksta – tako se da se stara
	// rutina poziva samo kada je stvarno doslo do prekida
	if (!timer_flag) {
		#ifndef BCC_BLOCK_IGNORE
		asm int 60h
		#endif
		if (cnt>0) cnt--;
		sleeping->update();
	}
	else timer_flag=0;
	static volatile unsigned tsp, tss, tbp;
	if ((PCB::running->timeSlice!=0 && cnt == 0) || context_switch) {
		if (flag == 0) {
			context_switch = 0;
#ifndef BCC_BLOCK_IGNORE
			asm{
				// cuva sp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}
#endif

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

			if(PCB::running != idle->myPCB && PCB::running->stanje==READY)Scheduler::put((PCB*)PCB::running);
			PCB::running = Scheduler::get();	// Scheduler
			if(PCB::running==NULL){
				PCB::running = idle->myPCB;
			}

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			cnt = PCB::running->timeSlice;

#ifndef BCC_BLOCK_IGNORE
			asm{
				mov sp, tsp   // restore sp
				mov ss, tss
				mov bp, tbp
			}
#endif
		}
		else context_switch = 1;
	}
}

void System::dispatch() {
#ifndef BCC_BLOCK_IGNORE
	asm cli;
#endif
	context_switch = 1;
	timer_flag = 1;
	timer();
#ifndef BCC_BLOCK_IGNORE
	asm sti;
#endif
}

void System::sleep(Time timeToSleep){

	if(timeToSleep > 0) {
		lock
		sleeping->put((PCB*)PCB::running,timeToSleep);
		PCB::running->stanje = BLOCKED;
		unlock
	}
	dispatch();
}

void System::system_lock()
{
	flag++;
}

void System::system_unlock()
{
	if (--flag == 0 && context_switch == 1) dispatch();
}


int System::wakeup(ID id){
	return sleeping->wakeup(id);
}

void System::removeFromSleeplist(ID id){
	sleeping->remove(id);
}

volatile int System::context_switch=0;
volatile Time System::cnt=defaultTimeSlice;
volatile unsigned System::flag=0;
volatile unsigned System::timer_flag=0;
volatile unsigned System::sem_flag=0;
pInterrupt System::oldISR=NULL;
PCB* System::main=NULL;
Idle* System::idle=NULL;
SleepList* System::sleeping=NULL;
