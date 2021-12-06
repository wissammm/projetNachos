// exception.cc 
//      Entry point into the Nachos kernel from user programs.
//      There are two kinds of things that can cause control to
//      transfer back to here from user code:
//
//      syscall -- The user code explicitly requests to call a procedure
//      in the Nachos kernel.  Right now, the only function we support is
//      "Halt".
//
//      exceptions -- The user code does something that the CPU can't handle.
//      For instance, accessing memory that doesn't exist, arithmetic errors,
//      etc.  
//
//      Interrupts (which can also cause control to transfer from user
//      code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

#ifdef CHANGED
#include "userthread.h"
#include "fork.h"
#endif 
//----------------------------------------------------------------------
// UpdatePC : Increments the Program Counter register in order to resume
// the user program immediately after the "syscall" instruction.
//----------------------------------------------------------------------
static void
UpdatePC ()
{
    int pc = machine->ReadRegister (PCReg);
    machine->WriteRegister (PrevPCReg, pc);
    pc = machine->ReadRegister (NextPCReg);
    machine->WriteRegister (PCReg, pc);
    pc += 4;
    machine->WriteRegister (NextPCReg, pc);
}


//----------------------------------------------------------------------
// ExceptionHandler
//      Entry point into the Nachos kernel.  Called when a user program
//      is executing, and either does a syscall, or generates an addressing
//      or arithmetic exception.
//
//      For system calls, the following is the calling convention:
//
//      system call code -- r2
//              arg1 -- r4
//              arg2 -- r5
//              arg3 -- r6
//              arg4 -- r7
//
//      The result of the system call, if any, must be put back into r2. 
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//      "which" is the kind of exception.  The list of possible exceptions 
//      are in machine.h.
//----------------------------------------------------------------------

void ExceptionHandler (ExceptionType which)
{
    int type = machine->ReadRegister (2);
    int address = machine->registers[BadVAddrReg];

    switch (which)
      {
		  
	case SyscallException:
          {
			  DEBUG('s',"type :%d \n",type);
	    switch (type)
	      {
		case SC_Halt:
		  {
		    DEBUG ('s', "Shutdown, initiated by user program.\n");
		    interrupt->Powerdown ();
		    break;
		  }
		  #ifdef CHANGED
		  case SC_PutChar:
		  {
		    DEBUG ('s',"PutChar\n ");
		    consoledriver->PutChar(machine->ReadRegister(4));
		    break;
		  }
		  case SC_PutString:
		  {
			DEBUG ('s',"PutString\n ");
			int c = machine->ReadRegister (4); // recupération de la chaine de caractère
            char* to = new char[MAX_STRING_SIZE]; // buffer le +1 permet d'ajouter le caractere de fin de chaine
			
            int i = consoledriver->copyStringFromMachine(c, to, MAX_STRING_SIZE-1); // copie chaine mips vers chaine Linux
			consoledriver->PutString(to);
			
			
			int cpt=1; //compteur à 1 car on à déja écrit une fois
			while(i==-1){ // si il reste encore des charactères à lire
				i = consoledriver->copyStringFromMachine(c+((MAX_STRING_SIZE)*cpt), to, MAX_STRING_SIZE); //écrit MAX_STRING_SIZE plus loin à chaque tour de boucle
				consoledriver->PutString(to);
				cpt++;
			}
			free(to); //on libère la mémoire
			break;
		  }

		  case SC_GetChar:
		  {
		    DEBUG ('s',"GetChar\n ");
		    machine->WriteRegister(2,consoledriver->GetChar());
		    break;
		  }

		  case SC_GetString:
		  {
		    DEBUG ('s',"GetChar\n ");
		    int to = machine->ReadRegister(4);
            int size= machine->ReadRegister(5); 
            char *from = new char[size];
            consoledriver->GetString(from,size-1); 
            consoledriver->copyStringToMachine(from,to,size); 
            free(from);
            break;
		  }

		  case SC_Exit:
		  {

			  do_ForkExit();
			  break;
		  }

		  case SC_ThreadCreate:
		  {
			int func = machine->ReadRegister(4);
            int args = machine->ReadRegister(5);
            int res = do_ThreadCreate(func, args);
			if(res==-1){
				printf("Thread non créé");
		    	ASSERT(FALSE);
			}
            machine->WriteRegister(2,res);
            break;
		  }
		  
		  case SC_ThreadExit:
		  {
			  do_ThreadExit();
			  break;
		  }

		  case SC_ForkExec:
		  {
			  int c = machine->ReadRegister (4); // recupération de la chaine de caractère
              char* to = new char[100];
			  consoledriver->copyStringFromMachine(c ,to,99);
			  do_ForkExec(to);
			  break;
		  }


		  #endif 
		default:
		  {
		    printf("Unimplemented system call %d\n", type);
		    ASSERT(FALSE);
		  }
	      }

	    // Do not forget to increment the pc before returning!
	    UpdatePC ();
	    break;
	  }

	case PageFaultException:
	  if (!address) {
	    printf("NULL dereference at PC %x!\n", machine->registers[PCReg]);
	    ASSERT (FALSE);
	  } else {
	    printf ("Page Fault at address %x at PC %x\n", address, machine->registers[PCReg]);
	    ASSERT (FALSE);	// For now
	  }
	  break;

	case ReadOnlyException:
	  printf ("Read-Only at address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case BusErrorException:
	  printf ("Invalid physical address at address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case AddressErrorException:
	  printf ("Invalid address %x at PC %x\n", address, machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case OverflowException:
	  printf ("Overflow at PC %x\n", machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	case IllegalInstrException:
	  printf ("Illegal instruction at PC %x\n", machine->registers[PCReg]);
	  ASSERT (FALSE);	// For now
	  break;

	default:
	  printf ("Unexpected user mode exception %d %d %x at PC %x\n", which, type, address, machine->registers[PCReg]);
	  ASSERT (FALSE);
	  break;
      }
}
