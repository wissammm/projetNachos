#include "machine.h"
#include "addrspace.h"
#include "bitmap.h"

#ifndef PAGEPROVIDER_H
#define PAGEPROVIDER_H

class PageProvider :dontcopythis  {
    public:
        PageProvider();
        ~PageProvider();
        int GetEmptyPage();
        void RealeasePage(int page);
        int NumAvailPages();
    private:
        BitMap* map;
};

#endif
