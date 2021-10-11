#include "syscall.h"
int returnvalue(){
    Exit("ici c'est paris");
}

int main ()
{
    
    PutString ("j'adore les cours de systeme d'exploitation, je passe toujours un bon moment car la matiere est facile, intuitive et j'ai jamais de core dumped");
    PutString (returnvalue());
    Exit(0);
}
