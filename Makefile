HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
SRCS=src/*cpp
CPPFLAGS=--std=c++11
CPPFLAGS += `pkg-config --cflags protobuf`
GRPC_ROOT=/Users/baidu/Documents/workspace/grpc
GRPC_INCLUDE=$(GRPC_ROOT)/include
GRPC_LIBS=-L$(GRPC_ROOT)/libs/opt -lgrpc -lgrpc++
DY_GRPC_LIBS=$(GRPC_ROOT)/libs/opt/libgrpc.dylib
LIBS=`pkg-config --libs protobuf` $(GRPC_LIBS)
LIBS += -L./src/interface -linterface
INCLUDES = -I./src -I./src/interface -I$(GRPC_INCLUDE)

ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib $(LIBS) \
           -lgrpc++_reflection\
           -ldl
else
LDFLAGS += -L/usr/local/lib $(LIBS) \
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl
endif

all: interface
	$(CXX) $(SRCS) -o main $(CPPFLAGS) $(INCLUDES) $(LDFLAGS)
.PHONY: tools
tools:
	$(CXX) tools/*cpp -o kv_client $(CPPFLAGS) $(INCLUDES) $(LDFLAGS)

interface:
	-make -C src/interface

clean:
	@rm main
	@rm kv_client 
