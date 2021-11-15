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

    DEBUG ('a', "Initializing stack register to 0x%x\n",
	   currentThread->space->AllocateUserStack() * PageSize - 16);

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
    currentThread->Finish();
    return 0;
}