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

$ ./main 256
Running on device: Intel(R) UHD Graphics 750 [0x4c8a]
num_items: 268435456
dummy malloc to avoid some overhead(?)    0.042 ms      23952.670 GB/s
malloc 1          0.004 ms      281373.101 GB/s
malloc 2          0.003 ms      371609.067 GB/s
memset          178.963 ms        5.588 GB/s
memcpy h->d      75.688 ms       13.212 GB/s
memcpy d->d     172.949 ms        5.782 GB/s
kernel           52.545 ms       19.031 GB/s
memcpy d->h      71.945 ms       13.900 GB/s
sycl event: kernel,52.049632 ms
-1,-2,-3,-4,-5,-6,-7,-8,-9,-10,...,-268435447,-268435448,-268435449,-268435450,-268435451,-268435452,-268435453,-268435454,-268435455,-268435456,

```

## References

- Intel DPCPP: https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html#gs.u2aerz
- SYCL revision 4: https://www.isus.jp/wp-content/uploads/pdf/sycl-2020_JA.pdf
