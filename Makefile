HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
SRCS=src/*cpp
CPPFLAGS=--std=c++11
CPPFLAGS += `pkg-config --cflags protobuf`
include Makefile.libs
DY_GRPC_LIBS=$(GRPC_ROOT)/libs/opt/libgrpc.dylib

ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib $(LIBS) \
           -ldl
else
LDFLAGS += -L/usr/local/lib $(LIBS) \
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed \
           -ldl -Wl,-rpath=$(GRPC_ROOT)/libs/opt
endif

all: interface
	$(CXX) $(SRCS) -o main $(CPPFLAGS) $(INCLUDES) $(LDFLAGS) -rpath $(GRPC_ROOT)/libs/opt -DUSING_GRPC
.PHONY: tools
tools:
	$(CXX) tools/*cpp -o kv_client $(CPPFLAGS) $(INCLUDES) $(LDFLAGS)
	mv kv_client tools/

interface:
	-make -C src/interface

clean:
	-@rm main &>/dev/null
	-@rm kv_client &>/dev/null 
