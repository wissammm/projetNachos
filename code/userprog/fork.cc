#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "thread.h"
#include "fork.h"


int nbProcessus =1;

static void StartUserProg(void * nullType){
     DEBUG('p',"Start user Prog \n");
     
    spaceProcessus->InitRegisters ();	// set the initial register values
    spaceProcessus->RestoreState ();	// load page table register

    machine->DumpMem ("memory.svg");
    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
}

int 
do_ForkExec(const char *s){
    OpenFile *executable = fileSystem->Open (s);
    

    if (executable == NULL)
      {
	  SetColor (stdout, ColorRed);
	  SetBold (stdout);
	  printf ("Unable to open file %s\n", s);
	  ClearColor (stdout);
	  return 0;
      }
    DEBUG('p',"do fork exec \n");
    spaceProcessus = new AddrSpace (executable);
    Thread *t = new Thread("Nouveau processus");
    t->space = spaceProcessus;
    nbProcessus ++;
    t->Start(StartUserProg, NULL);
    DEBUG('p',"do fork exec FIN %x , NextPcReg : %x\n", machine->ReadRegister(PCReg) , machine->ReadRegister(NextPCReg));
    DEBUG('p',"registre 31 : %x \n ", machine->ReadRegister(31));
    delete executable;		// close file

    
    return 0;
}

int
do_ForkExit(){
  nbProcessus--;
  if(nbProcessus==0){
      //Exit(machine->ReadRegister(4));
      interrupt->Powerdown();
  }
  currentThread->Finish();
  return 0;
}