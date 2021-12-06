#include "syscall.h"

int main(){
    ForkExec("/home/wissam/Documents/projetNachos/code/test/putchar");
    PutChar('c');
    //while(1);
    Exit(0);
}