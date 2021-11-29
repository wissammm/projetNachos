#include "machine.h"
#include "addrspace.h"
#include "bitmap.h"


PageProvider(){
    map = new BitMap( NumPhysPages);
    
}

int GetEmptyPage(){
    return 0;
}

bool RealeasePage(){
    return true;
}

int NumAvailPages(){
    return 0;
}