#include "system.h"
#include "console.h"
#include "addrspace.h"
#include "thread.h"
#include "fork.h"


int StartUserProg(){
    spaceProcessus->InitRegisters ();	// set the initial register values
    spaceProcessus->RestoreState ();	// load page table register

    machine->DumpMem ("memory.svg");
    machine->Run ();		// jump to the user progam
    ASSERT (FALSE);		// machine->Run never returns;
    // the address space exits
    // by doing the syscall "exit"
    return 0;
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
    spaceProcessus = new AddrSpace (executable);
    Thread *t = new Thread("Nouveau processus");
    t->space = spaceProcessus;
    t->Start(StartUserProg, NULL);
    

    delete executable;		// close file

    
    return 0;
}