# C Language Simmulation code for Fully Associative &Set Associative Cache

## Getting Started

    A Fully Associative & Set Associative Cache simmulation based on input arguments. Curently 4 Set 2 Way cache Simmulation available here

### Compilation in GCC supported unix environment

```c
    gcc -Wall cache.c -o cache
```

#### Steps to Run above code for Fully Associative

For **Fully Associative**

```c
    ./cache -Fa 16 trace.txt
```

* Argument 1 can be **Fa | fa | fA | FA** to select Fully Associative
* Argument 2 can be 1 to 4-digit number of range 1 - 9999 and is optional too. Default selects 16 blocks of cache size

For **Set Associative**

```c
    ./cache -sA trace.txt
        or
    ./cache -sA 4-2 trace.txt
```

* Here 4 sets 2 ways functionality implemented as default for now
* K-N ways is not implemented, So Arguments 2 can be anything

**Usage** : ./*output_file* -[* DM | fa | Sa *] [*K-N* ways] | *cache size for FA* *input_file*