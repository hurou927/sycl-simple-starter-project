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
$ ./main
```

## References

- Intel DPCPP: https://www.intel.com/content/www/us/en/developer/tools/oneapi/dpc-compiler.html#gs.u2aerz
- SYCL revision 4: https://www.isus.jp/wp-content/uploads/pdf/sycl-2020_JA.pdf
