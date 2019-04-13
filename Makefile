HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
SRCS=src/*cpp
CPPFLAGS=--std=c++11
CPPFLAGS += `pkg-config --cflags protobuf`
#GLOG & GFLAGS
GLOG_GFLAG_INCLUDE = -I/usr/local/opt/gflags/include
GLOG_GFLAG_LIBS = /usr/local/opt/gflags/lib/libgflags.dylib
# LOCAL LIBS
LOCAL_INCLUDES = -I./src -I./src/interface
RPC_TYPE=BRPC
# RPC_TYPE=GRPC
# LOCAL_LIBS = -L./src/interface -linterface_brpc
ifeq ($(RPC_TYPE), GRPC)
	LOCAL_LIBS = -L./src/interface -linterface_grpc
	include Makefile.grpc_libs
	SWITCH_FLAGS=-DUSING_GRPC
	GLOG_GFLAG_INCLUDE = -I/usr/local/opt/glog/include $(GLOG_GFLAG_INCLUDE) 
	GLOG_GFLAG_LIBS = /usr/local/opt/glog/lib/libglog.dylib $(GLOG_GFLAG_LIBS) 
else
	LOCAL_LIBS = -L./src/interface -linterface_brpc
	include Makefile.brpc_libs
	SWITCH_FLAGS=-DUSING_BRPC
endif

ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib $(LIBS) \
           -ldl
else
LDFLAGS += -L/usr/local/lib $(LIBS) \
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed \
           -ldl -Wl,-rpath=$(GRPC_ROOT)/libs/opt
endif

all: interface
	$(CXX) $(SRCS) -o main $(CPPFLAGS) $(INCLUDES) $(LDFLAGS) $(LOCAL_LIBS) $(SWITCH_FLAGS) 
.PHONY: tools
tools:
	$(CXX) tools/*cpp -o kv_client $(CPPFLAGS) $(INCLUDES) $(LDFLAGS)
	mv kv_client tools/

interface:
	-make -C src/interface -f Makefile.grpc

clean:
	-@rm main &>/dev/null
	-@rm kv_client &>/dev/null 
