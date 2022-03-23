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
num_items: 268435456
kernel execution time without submission: 52.1155 ms
dummy malloc to avoid some overhead(?)    0.040 24979.392 GB/s
malloc 1          0.004 238038.562 GB/s
malloc 2          0.004 276319.425 GB/s
memset          176.275   5.673 GB/s
memcpy h->d      67.183  14.885 GB/s
memcpy d->d     170.441   5.867 GB/s
kernel           52.611  19.008 GB/s
memcpy d->h      62.577  15.980 GB/s
-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,...,-268435447,-268435448,-268435449,-268435450,-268435451,-268435452,-268435453,-268435454,-268435455,-268435456,
```

## References

- Intel DPCPP: https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html#gs.u2aerz
- SYCL revision 4: https://www.isus.jp/wp-content/uploads/pdf/sycl-2020_JA.pdf
