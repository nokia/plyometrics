#pragma once

#ifndef __OPTIMIZE__
    #warning debug
#endif

namespace nbench
{

void escape(void* p)
{
    asm volatile("" : : "g"(p) : "memory");
}

}
