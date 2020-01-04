Memory fragmentation
--------------------
There are two common ways to allocate memory in C++. _Stack_ and _heap_ allocation. There are number of differences between them but we want to focus mainly on one. _Heap_ allocated memory can be deallocated in different order to the allocations. This means that holes are created in the used memory. Good allocator algorithms will of course try to reuse this holes to conserve the memory but this will also mean that consecutive allocations might get fragmented.

[`memory_fragmentation.cpp`](memory_fragmentation.cpp) shows the impact of the memory fragmentation on some STL containers. Because allocated elements might be far apart of each other, the program will probably generate a lot of _data cache misses_.

Not all of STL containers will be impacted, `std::vector` for example, keeps the elements in contiguous sequence. _Open addressing_ hash tables will also most likely behave better than their counterparts.


CPU branch prediction
---------------------
[`cpu_branch_prediction.cpp`](cpu_branch_prediction.cpp) shows behavior of _CPU branch predictor_ and how you can speed it up in some cases. Usually, virtual function call in C++ looks like this:

```
mov    (%rdi),%rax
callq  *(%rax)
```

This is problematic because it is impossible for CPU to even guess where the destination is going to be. Modern platform tries to overcome this by implementing _branch prediction_ algorithms which maintains some sort of history of the branch destination (it does `jmp`, `ret` or anything else that takes computed address). This works pretty well but it depends on how good is the algorithm and how big is the history buffer so it's safe to assume that it will behave better on some modern `CORE i7` than on say, `ARMv7`.

Dynamic branch predictor will do nothing for indirect calls when there is no history or the buffer is overflowed. We can observe this by running `dynamic_branch_prediction` benchmark and check branch related PMU counters:

```
$ perf stat -e branches,branch-misses ./build/examples/cpu_branch_prediction -b dynamic_branch_prediction
[...]
       206 999 639      branches                                                    
        67 297 762      branch-misses             #   32,51% of all branches
```

_Static predictor_ is a different thing though. Consider this branch:

```
cmp    %rdi,%rax
je     1188 <foo+0x28>
```

There are only two options so CPU can at least try to guess which one will be taken. Usually it's assumed that branch is not taken, i.e. the jump will not occur. We can use that in cases where interfaces are introduced only to simplify writing unit tests and they have only two derived classes - implementation and the mock. The idea can be simply expressed in the pseudo-code:

```
class implementation
class mock

if class of an object == implementation:
    implementation(object).foo()
else
    object.foo()
```

Let's see how it works then:

```
$ perf stat -e branches,branch-misses ./build/examples/cpu_branch_prediction -b hinted_branch_prediction
[...]
       355 480 606     branches                                                    
            52 910     branch-misses             #    0,01% of all branches        
```

This helps with two things:
 - static prediction will usually be sufficient so we don't depend on branch history
 - compiler can do inlining (which will then enable other imptimizations)

The caveat is that the code will probably be bigger which will cause higher load of the _instruction cache_ so as always, **you should measure** your particular system before deploying this solution.


Out-of-order execution
----------------------
`cpu_phenmenons.cpp` contains two benchmarks which together show _out-of-order execution_ capability of the CPU. Second test is expected to be faster because second modulo operation doesn't depend on any previously calculated value therefore it can be scheduled in separate pipeline.