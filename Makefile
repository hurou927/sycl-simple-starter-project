################################
# C++ Make
#################################

CXX=dpcpp
CXXFLAGS=-fsycl-targets=spir64_gen -Xs "-device tgllp" -O3  -std=c++20 -I./include
CXXLIBS=
CXXSRC=$(wildcard *.cpp)
CXXTARGET=$(basename $(CXXSRC))

.PHONY: all
all:$(CXXTARGET)
$(CXXDEP):%:%.cpp
	$(CXX) -o $* $^ $(CXXFLAGS) $(CXXLIBS) 

#################################
# all : force all source code compiler
#################################
.PHONY: force
force: clean $(CXXTARGET)

#################################
# clean : make clean
#################################
.PHONY: clean
clean:
	rm -f $(CXXTARGET)
