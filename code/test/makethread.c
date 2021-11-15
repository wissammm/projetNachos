#include "syscall.h"

void Putchar(){
    PutChar('c');
    ThreadExit();
}

int main(){
    
    ThreadCreate(Putchar,0);
    PutChar('c');
    while(1);

}