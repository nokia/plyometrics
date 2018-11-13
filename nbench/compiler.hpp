#pragma once

#ifndef __OPTIMIZE__
    #warning it appears that you are building with -O0
#endif

namespace nbench
{

void escape(void* p)
{
    asm volatile("" : : "g"(p) : "memory");
}

}
