#include "copyright.h"
#include "utility.h"
#include "synch.h"
#include "addrspace.h"


static AddrSpace *spaceProcessus;
int do_ForkExec(const char *s );
static void StartUserProg(void * nullType);
int do_ForkExit();