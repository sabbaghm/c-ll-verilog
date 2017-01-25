CC = g++
INCLUDE = .
CFLAGS = -I$(INCLUDE) -fPIC -shared `llvm-config --cppflags` -std=c++11

ALLSRCS = *.cc

TARGETALL = verilog.so

all: $(TARGETALL)
$(TARGETALL): $(ALLSRCS)
	$(CC) $(CFLAGS) $(INCLUDES) $(ALLSRCS) -o $(TARGETALL)

clean:
	$(RM) $(TARGETBISON) $(TARGETFLEX) $(TARGETALL)
