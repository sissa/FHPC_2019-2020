# Exercise on memory aliasing and some basic cache elements



## - 1 - Playing with caches
#### 1.1 the Memory Mountain

#### 1.2 get your L1 cache size

#### 1.3 hot and cold fields in data structures

This example is about the difference in performances that you can get just by modifying the data layout.
In this case, the focus is on splitting _hot_ and _cold_ data fields in structure.

## - 2 - memory aliasing

#### first example, `memory_alising_1/`

###### **_Compilation_**

Compilation should be straight forward:

```bash
cc -std=c11 -o pointers_aliasing_<a|b|c> pointers_aliasing_<a|b|c>.c
```

where `<a|b|c>` stands for "a choice among a, b, c".
Compile with and without simple optimisation (i.e. just using -On argument without other special option):

```bash
cc -std=c11 -On -o pointers_aliasing_<a|b|c> pointers_aliasing_<a|b|c>.c
```

where the maximum `n`  level of `-O` option is 3 for `gcc` and `icc`, 4 for `pgcc`.

Try with different compilers (Intel's and PGI's compilers are free to download).

TIPS for compilation:

-  `gcc`
  - you may want to add `-march=native`
-  `icc`, `pgcc`
  - you can add `-fast`
-  `pgcc`
  - standard `c11` is specified as `-c11` instead of `-std=c11`



You find 2 simple bash scripts: `compile_all` and `run`. The former allows you to compile the 

###### results with different compilers
|  | gcc |  | pgi |  | icc |  |
| :- | :-----: | :------: | :------: | -------- | -------- | -------- |
| **case** | ***no-opt*** | ***opt*** | ***no-opt*** | ***opt*** | ***no-opt*** | ***opt*** |
| a    | 0.028 | 0.0051 | 0.024 | 0.0071 | 0.004 | 0.0078 |
| b    | 0.028 | 0.0051 | 0.024 | 0.0032 | 0.004 | 0.0031 |
| c    | 0.030 | 0.0029 | 0.025 | 0.0029 | 0.0029 | 0.0017 |


#### second example, `memory_aliasing_2`
###### **_Compilation_**

As in previous case, the compilation should be straight forward:

```bash
cc -std=c11 -o aliasing driver.c loop.c
```

Compile with and without optimization (see tips for the precious examples).

Try with different compilers.

|  | gcc |  | pgi |  | icc |  |
| :- | :-----: | :------: | :------: | :------: | :------: | :------: |
| **case** | ***no-opt*** | ***opt*** | ***no-opt*** | ***opt*** | ***no-opt*** | ***opt*** |
| aliasing | 0.032 | 0.009 | 0.027 | 0.0093 | 0.01 | 0.0095 |
| no alias | 0.032 | 0.0075 | 0.027 | 0.0093 | 0.01 | 0.0093 |