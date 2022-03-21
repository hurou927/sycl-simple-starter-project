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

$ ./main
Compilation from IR - skipping loading of FCL
Build succeeded.
Running on device: Intel(R) UHD Graphics 750 [0x4c8a]
num_items: 1024
1. malloc. 0.513466 ms
2. memcpy h->d. 0.183394 ms
3. kernel. 0.114257 ms
4. memcpy d -> h. 0.173909 ms
kernel execution time without submission: 0.016102 ms
-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,...,-1015,-1016,-1017,-1018,-1019,-1020,-1021,-1022,-1023,-1024,

```

## References

- Intel DPCPP: https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html#gs.u2aerz
- SYCL revision 4: https://www.isus.jp/wp-content/uploads/pdf/sycl-2020_JA.pdf
