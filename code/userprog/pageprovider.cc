#include "machine.h"
#include "addrspace.h"
#include "bitmap.h"
#include "pageprovider.h"
#include "utility.h"
#include "system.h"


PageProvider :: PageProvider(){
    map = new BitMap(64);
}

PageProvider::~PageProvider(){
    delete[] map;
}

int 
PageProvider::GetEmptyPage(){
    //int page = map->Find();
    
    //test random 
    int k= rand() % 64;
    while(map->Test(k) ){
        k = rand() % 64;
    }
    map->Mark(k);
    int page = k;
    
    int adresse = page * PageSize;
    char *mem = &(machine->mainMemory[adresse]);
    memset(mem, 0, PageSize);
    return page;
}

void
PageProvider::RealeasePage(int page){
    map->Clear(page);
}

int 
PageProvider::NumAvailPages(){
    return map->NumClear();
}