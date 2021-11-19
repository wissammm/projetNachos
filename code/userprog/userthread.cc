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
    currentThread->space->p->P(); //on protège la recherche de place dans bitmap
    currentThread->SetId(currentThread->space->MapFind());
    currentThread->space->p->V();
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

    machine->WriteRegister(4, shmurtz->arg); //on met les argument dans le registre 4
    delete(shmurtz);
    currentThread->Yield();
    machine->DumpMem("threads.svg");
    
    machine->Run();
    
}

int do_ThreadCreate(int f,int arg){

    currentThread->space->sem->P(); //enleve une place
    Shmurtz *shm = new Shmurtz;
    shm->f=f;
    shm->arg=arg;
    Thread *newThread = new Thread ("fonction");
    if(newThread == NULL) {
		DEBUG('f',"thread null");
		return -1;
	}
    newThread->space = currentThread->space; //le thread et le processus qui l'appelle on le même espace d'adressage
    currentThread->space->AddThread(); //incrémente le nombre de thread
    newThread->Start(StartUserThread,shm); //lance le nouveau thread
    
    currentThread->Yield();
    return 0;
}

int do_ThreadExit(){
    
    currentThread->space->ClearMap(currentThread->GetId()); //supprime le thread du bitmap
    currentThread->space->RemoveThread(); //enleve un thread au compteur
    if(currentThread->space->GetNumberThread()==0){
        delete(currentThread->space->p);
        delete(currentThread->space->map);
        delete(currentThread->space->sem);
        interrupt->Powerdown();
        return 0;
    }
    currentThread->space->sem->V(); //ouvre une place pour un thread en attente 
    currentThread->Finish();
    return 0;
}