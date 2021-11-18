#include "syscall.h"

void print(char arg){
    volatile int x ;
    for(x= 0; x<3; x++){
        PutChar('a');
        PutString("\n");
    }
    ThreadExit();
}

int main (){
    char arg = 'k';
    void *f = print;
    int i;
    for ( i= 0; i<5; i++){
        ThreadCreate(f, arg);
    }
    PutString("Retour au main \n");
    ThreadExit();
}