#include "syscall.h"

int main(){
    char s[100];
    GetString(&s,20);
    PutString(s);
    Halt();
}