#include <iostream.h>
#include "system.h"
//#include "thread.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	System::inic();

	int ret = userMain(argc, argv);

	System::restore();

	return ret;
}



/*



#include "klasaa.cpp"
#include "klasab.cpp"
#include "system.h"
#include <iostream.h>
#include "klasaw.cpp"
#include "test_kl.cpp"
#include "test_id.cpp"
#include "test_sem.cpp"

int main(){

	cout<<"U main"<<endl;
	System::inic();
	cout<<"Sistem je inicijalizovan"<<endl;


*/
/*
	Test_thread* niz[20];
	Time vreme[20]={10,20,30,10,50,40,50,70,90,60,70,80,100,100,10,5,1,10,18,20};

	for (int i=0;i<20;i++){
		niz[i]=new Test_thread(defaultStackSize,defaultTimeSlice,vreme[i]);

	}

	for(int j=0;j<20;j++){
		niz[j]->start();
		//cout<<"Pokrenuta nit "<<j+2<<endl;
	}

	for(int k=0;k<20;k++){
		delete niz[k];
	}
*/
/*
	TestSem* niz[5];
	Time vreme[5]={50,40,0,20,10};

	Semaphore *s=new Semaphore(0);

	int num[5]={1,1,1,-5,0};


	for (int i=0;i<5;i++){
		niz[i]=new TestSem(defaultStackSize, 200, s, vreme[i], num[i]);
	}



	for(int j=0;j<5;j++){
		niz[j]->start();
		//cout<<"Pokrenuta nit "<<j+2<<endl;
	}

	for(int k=0;k<5;k++){
		delete niz[k];
	}

	cout<<"s="<<s->val()<<endl;

	cout<<"Happy End"<<endl;
	System::restore();
	cout<<"Kraj";
	return 0;
}




*/
