#include "syscall.h"

void print(char arg){
    volatile int x ;
    for(x= 0; x<3; x++){
        PutChar(arg);
        
    }
    ThreadExit();
}

void print2(char string[]){
    
    PutString(string);
    ThreadExit();
}

int main (){
    PutChar('c');
    char arg = 'k';
    void *f = print;
    int i;
    for ( i= 0; i<6; i++){
        ThreadCreate(f, arg);
    }

    char chaine[] = "Salut";

    ThreadCreate(print2, chaine);
    ThreadExit();
}