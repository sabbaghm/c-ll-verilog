# Update: Support New Pass Manager (tested on LLVM 17/18) and migrate to CPP
# @author Mohamed Bouaziz (mohamed.bouaziz@kaust.edu.sa), KAUST
# @date   Oct, 2023


CXX = clang++
CXXFLAGS = -fPIC -shared -fno-rtti $(shell llvm-config --cppflags) -std=c++17
LDFLAGS = 

ALLSRCS = *.cpp

TARGET = verilog.so

all: $(TARGET)

$(TARGET): $(ALLSRCS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

clean:
	$(RM) $(OBJS) $(TARGET)
