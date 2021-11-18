#include "userthread.h"
#include "system.h"




struct Shmurtz { //structure à passer en parametre 
    int f;
    int arg;
};



static void StartUserThread(void* shm){

    
    int i;
    Shmurtz *shmurtz;
    shmurtz = (Shmurtz *) shm;
    currentThread->SetId(currentThread->space->MapFind());
    for (i = 0; i < NumTotalRegs; i++){
        machine->WriteRegister (i, 0); // on met tout les registres à 0
    }
	
    // Initial program counter -- must be location of "Start"
    machine->WriteRegister (PCReg , shmurtz->f); //on donne notre fonction 

    // Need to also tell MIPS where next instruction is, because
    // of branch delay possibility
    machine->WriteRegister (NextPCReg, shmurtz->f + 4); //on initialise la prochaine instruction 
    
    // Set the stack register to the end of the address space, where we
    // allocated the stack; but subtract off a bit, to make sure we don't
    // accidentally reference off the end!

    machine->WriteRegister (StackReg, currentThread->space->AllocateUserStack(currentThread->GetId()) );

    machine->WriteRegister(4, shmurtz->arg);
    currentThread->Yield();
    machine->Run();
    
}

int do_ThreadCreate(int f,int arg){
    //DEBUG('f',"Do Thread Create\n");
    currentThread->space->sem->P();
    Shmurtz *shm = new Shmurtz;
    shm->f=f;
    shm->arg=arg;
    Thread *newThread = new Thread ("fonction");
    newThread->space = currentThread->space;
    currentThread->space->AddThread();
    newThread->Start(StartUserThread,shm);
    
    currentThread->Yield();
    return 0;
}

int do_ThreadExit(){
    fprintf(stderr, "Exit Thread %s\n", currentThread->getName());
    currentThread->space->ClearMap(currentThread->GetId());
    currentThread->space->RemoveThread();
    if(currentThread->space->GetNumberThread()==0){
        interrupt->Powerdown();
        return 0;
    }
    currentThread->space->sem->V();
    currentThread->Finish();
    return 0;
}