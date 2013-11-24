CXX=g++

OBJS=vm 
#CXXFLAGS=-g -Wall `llvm-config --cxxflags --ldflags --libs` -lpthread -ldl
CXXFLAGS=-g -Wall `llvm-config --cxxflags --libs` `llvm-config --ldflags`

.SUFFIXES: .cpp .o

.cpp.o:
	$(CXX) -c $< -o $@ $(CXXFLAGS)

vm: vm.o
	$(CXX) -o $@ $< $(CXXFLAGS) 

clean: 
	rm -f $(OBJS) *.o
