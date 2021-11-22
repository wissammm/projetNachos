#include "syscall.h"
void print(char c, int n){
    int i =0;
    for(i=0;i<n;i++){
        PutChar(c);
    }
}
int
main(){
    print('a',4);
    Exit(5);
}