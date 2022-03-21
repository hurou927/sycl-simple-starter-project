# SYCL: simple starter project

Simple SYCL project using USM, explicit data management

## Setup

```sh
# If you use OneApi
$ . /opt/intel/oneapi/setvars.sh
```

### Specify device architecture

Fix compiler flags in Makefile

e.g.)
- Intel Integrated Graphics(Tiger Lake)
  - `-fsycl-targets=spir64_gen -Xs "-device tgllp"`

Ref: https://www.intel.com/content/www/us/en/develop/documentation/oneapi-dpcpp-cpp-compiler-dev-guide-and-reference/top/compilation/ahead-of-time-compilation.html

## Build & Run
```sh
$ make
dpcpp -fsycl-targets=spir64_gen -Xs "-device tgllp" -O3  -std=c++20 -I./include    main.cpp   -o main
Compilation from IR - skipping loading of FCL
Build succeeded.

$ ./main 1024
Running on device: Intel(R) UHD Graphics 750 [0x4c8a]
num_items: 1048576
1. dummy malloc to avoid some overhead(?). 0.022387 ms
2. malloc 1. 0.00364 ms
3. malloc 2. 0.003461 ms
4. memset. 1.16675 ms
5. memcpy h->d. 0.337778 ms
6. memcpy d->d. 0.62882 ms
7. kernel. 0.269644 ms
8. memcpy d->h. 0.295388 ms
kernel execution time without submission: 0.150894 ms
-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,...,-1048567,-1048568,-1048569,-1048570,-1048571,-1048572,-1048573,-1048574,-1048575,-1048576,

```

## References

- Intel DPCPP: https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html#gs.u2aerz
- SYCL revision 4: https://www.isus.jp/wp-content/uploads/pdf/sycl-2020_JA.pdf
