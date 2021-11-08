
#include "system.h"
#include "syscall.h"
#include "thread.h"
#include "machine.h"
#include <thread.h>
#include "copyright.h"
#include "synch.h"
#include <machine.h>

struct Shmurtz { //structure à passer en parametre 
    int f;
    int arg;
};



int do_ThreadCreate(int f,int arg){
    Shmurtz *shm = new Shmurtz;
    shm->f=f;
    shm->arg=arg;
    Thread newThread = new Thread;
    newThread->Start(StartUserThread,shm)
    return 0;
}

static void StartUserThread(void* shmurtz){
    int i;

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
    machine->WriteRegister (StackReg, numPages * PageSize - 256);

    machine->run();
}