#include "copyright.h"
#include "utility.h"
#include "synch.h"
#include "addrspace.h"


static AddrSpace *spaceProcessus;
int do_ForkExec(const char *s );
int StartUserProg(AddrSpace spaceProcessus);