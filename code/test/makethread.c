#include "syscall.h"

void Putchar(char c){
    
    PutString("baba au rhum");
    ThreadExit();
}

int main(){
    
    ThreadCreate(Putchar,'b'); 
    PutChar('c');
    ThreadExit();
    Exit(0); //les deux ne termine pas nachos

}