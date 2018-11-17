#
# Copyright 2015 gRPC authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

HOST_SYSTEM = $(shell uname | cut -f 1 -d_)
SYSTEM ?= $(HOST_SYSTEM)
CXX = g++
CPPFLAGS += `pkg-config --cflags protobuf`
GRPC_ROOT=/Users/baidu/Documents/workspace/grpc
GRPC_INCLUDE=$(GRPC_ROOT)/include
GRPC_LIBS=-L$(GRPC_ROOT)/libs/opt -lgrpc -lgrpc++
DY_GRPC_LIBS=$(GRPC_ROOT)/libs/opt/libgrpc.dylib
CPPFLAGS += -I$(GRPC_INCLUDE)
CXXFLAGS += -std=c++11
LIBS=`pkg-config --libs protobuf` $(GRPC_LIBS)
ifeq ($(SYSTEM),Darwin)
LDFLAGS += -L/usr/local/lib $(LIBS) \
           -lgrpc++_reflection\
           -ldl
else
LDFLAGS += -L/usr/local/lib $(LIBS) \
           -Wl,--no-as-needed -lgrpc++_reflection -Wl,--as-needed\
           -ldl
endif
PROTOC = protoc
GRPC_CPP_PLUGIN = $(GRPC_ROOT)/bins/opt/grpc_cpp_plugin 
PROTOS_PATH = ./ 

all: service.pb.o service.grpc.pb.o 
	ar rc libinterface.a $^

vpath %.proto $(PROTOS_PATH)

.PRECIOUS: %.grpc.pb.cc
%.grpc.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --grpc_out=. --plugin=protoc-gen-grpc=$(GRPC_CPP_PLUGIN) $<

.PRECIOUS: %.pb.cc
%.pb.cc: %.proto
	$(PROTOC) -I $(PROTOS_PATH) --cpp_out=. $<

clean:
	rm -f *.o *.pb.cc *.pb.h
