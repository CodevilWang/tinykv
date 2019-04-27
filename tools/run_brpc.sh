#!/bin/bash
export LD_LIBRARY_PATH=../../brpc/_build/output/lib
./kv_client_brpc --kvsvc_host=list://10.94.213.126:50051
