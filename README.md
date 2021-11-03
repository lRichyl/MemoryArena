# MemoryArena
 A generic memory arena.
 
This is a basic memory arena allocator capable of allocating any type.
Currently fragmentation is not being handled but it is still functional.

Running on a Ryzen 3 3200G
Time ranges for allocating 100000 ints.
Arena           new
280us - 370us   5.8ms - 6.1ms
Maximum speed-up factor : 20x.
Minimun speed-up factor : 16x.

Compiled with g++ using optimization level O2 .
