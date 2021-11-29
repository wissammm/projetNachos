#include "machine.h"
#include "addrspace.h"
#include "bitmap.h"

BitMap* pages;
int GetEmptyPage();
bool RealeasePage();
int NumAvailPages();