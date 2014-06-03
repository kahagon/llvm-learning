CXX=g++
LLVM_CONFIG=llvm-config

CXXFLAGS=-g -Wall `${LLVM_CONFIG} --cxxflags` 
LDFLAGS=`${LLVM_CONFIG} --ldflags` `${LLVM_CONFIG} --libs` -lpthread -ldl -lncurses

OBJS=vm 

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) -c $< -o $@ $(CXXFLAGS)

vm: vm.o
	$(CXX) -o $@ $< $(LDFLAGS) 

clean: 
	rm -f $(OBJS) *.o
