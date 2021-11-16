#include "userthread.h"
#include "system.h"




struct Shmurtz { //structure à passer en parametre 
    int f;
    int arg;
};



static void StartUserThread(void* shm){
    DEBUG('f',"Start user thread debut");
    int i;
    Shmurtz *shmurtz;
    shmurtz = (Shmurtz *) shm;
    for (i = 0; i < NumTotalRegs; i++){
        machine->WriteRegister (i, 0); // on met tout les registres à 0
    }
	
    currentThread->space->AddThread();
    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg , shmurtz->f); //on donne notre fonction 

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, shmurtz->f + 4); //on initialise la prochaine instruction 
    DEBUG('f',"Start user thread debut %d", shmurtz->f);
    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!

    machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack() );

    machine->WriteRegister(4, shmurtz->arg);
    
    machine->Run();
    DEBUG('f',"Start user thread fin");
}

int do_ThreadCreate(int f,int arg){
    DEBUG('f',"do thread create debut");
    Shmurtz *shm = new Shmurtz;
    shm->f=f;
    shm->arg=arg;
    Thread *newThread = new Thread ("fonction");
    newThread->space = currentThread->space;
    newThread->Start(StartUserThread,shm);
    DEBUG('f'," %d", f);
    DEBUG('f',"do thread create fin");
    return 0;
}

int do_ThreadExit(){
    currentThread->space->RemoveThread();
    
    if(currentThread->space->GetNumberThread()==0){
        interrupt->Powerdown();
        return 0;
    }
    currentThread->Finish();
    return 0;
}