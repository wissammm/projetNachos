#include "syscall.h"

int main(){
    ForkExec("./putchar");
    while(1);
}