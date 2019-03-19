
#include <stdio.h>
#include <dlfcn.h>
#include <atomic>
#include <type_traits>

extern "C"
{

/**
 * dlsym needs malloc so we need to provide some before we can get the real one.
 * see: https://stackoverflow.com/a/10008252
 */
void* malloc_tmp_arena(size_t size)
{
    static std::aligned_storage<1024*32>::type tmp_arena;
    static char* tmp_arena_head = reinterpret_cast<char*>(&tmp_arena);

    // TODO: deal with the alignment
    void* ret = tmp_arena_head;
    tmp_arena_head += size;
    return ret;
}

void free_tmp_arena(void*)
{
}

namespace
{

using malloc_t = void*(size_t);
using free_t = void();

malloc_t* real_malloc = nullptr;
free_t* free_ptr = free_tmp_arena;

} // namespace

void *malloc(size_t size)
{

    if (real_malloc == nullptr)
    {
        real_malloc = malloc_tmp_arena;
        real_malloc = reinterpret_cast<malloc_t*>(dlsym(RTLD_NEXT, "malloc"));
    }

    void *p = NULL;
    fprintf(stderr, "malloc(%d) = ", size);
    p = (*real_malloc)(size);
    fprintf(stderr, "%p\n", p);
    return p;
}

void free(void*)
{
}

}
