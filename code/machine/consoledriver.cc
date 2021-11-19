
#include "copyright.h"
#include "system.h"
#include "consoledriver.h"
#include "synch.h"
#include <machine.h>

static Semaphore *readAvail;
static Semaphore *writeDone;
static void ReadAvailHandler(void *arg) { (void) arg; readAvail->V(); }
static void WriteDoneHandler(void *arg) { (void) arg; writeDone->V(); }
static Semaphore *sPut ;
static Semaphore *sGet ;
static Semaphore *sPutString ;


char ch;

ConsoleDriver::ConsoleDriver(const char*in,const char*out){
    readAvail = new Semaphore("read avail", 0);
    writeDone = new Semaphore("write done", 0);
    sPut= new Semaphore("sémaphore Put",1);
    sGet = new Semaphore("sémaphore Get",1);
    sPutString = new Semaphore("sémaphore PutString",1);
    console = new Console (in, out, ReadAvailHandler, WriteDoneHandler, NULL);
}
ConsoleDriver::~ConsoleDriver(){
    delete console;
    delete writeDone;
    delete readAvail;
    delete sPut;
    delete sGet;
}
void ConsoleDriver::PutChar(int ch){
    sPut->P();
    console->TX (ch);
    writeDone->P ();
    sPut->V();
}
int ConsoleDriver::GetChar(){
    sGet->P();
    readAvail->P ();	// wait for character to arrive
    ch = console->RX ();
    return ch;
    sGet->V();
}
void ConsoleDriver::PutString(const char s[]){
    
    int i = 0;
    while(s[i]!='\0'){ //tant que c'est pas la fin du tableau de charactères
        ConsoleDriver::PutChar(s[i]);
        i++;
    }
}

void ConsoleDriver::GetString(char*s,int n){
    char ch;
    for(int i = 0;i<n;i++){
        ch=ConsoleDriver::GetChar();
        if(ch==-1 ){ //EOF
            s[i]='\0';
            break;
        }
        else if((char)ch=='\n'){
            s[i]='\n';
            s[i+1]='\0';//on ajoute \n a la fin du string
            break;
        }
        s[i]=(char)ch;
    }
}

int ConsoleDriver::copyStringFromMachine(int from, char *to, unsigned size){ 
    unsigned int i=0;
    int buf; //buffer qui stockera 1 char
    while((i<size)&& machine->ReadMem(from+i,1,&buf)){//from +i est le prochain charactère en mémoire que l'on stock à buf 
		char c =(char)buf;
        to[i]= c; // on rajoute le charactère lu
        i++;
		if(c=='\0'){// fin de la chaine de charactère
			break;
		}
    }
	machine->ReadMem(from+i,1,&buf);
	if((i==size)&&((char)buf!='\0')){
		to[i]=(char)buf;
		return -1; // retourne -1 pour dire que la chaine n'est pas fini
	}
	else{
		to[i]='\0'; //sinon fin de chaine 
    	return i;
	}
    
}

void ConsoleDriver::copyStringToMachine(char *from, int to, unsigned size){
    unsigned int i = 0;
	int buf;

	while((i<size)&&(*(from+i)!='\0')){ // tant que c'est pas la fin de la chiane de charactèrers 
		buf = *(from+i);
		machine->WriteMem(to+i,1,buf); //écrire dans to res
		i++;
	}
	machine->WriteMem(to+i,1,'\0'); //fin de chaine 
}
