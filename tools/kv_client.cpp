// Copyright 2018 All Rights Reserved.
// Author: codevil.w.m@gmail.com (M W)

#include <assert.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <string>
#include <thread>

#include <grpc/grpc.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>
#include "service.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::ClientReaderWriter;
using grpc::ClientWriter;
using grpc::Status;
using TINYKV::KVService;

class KVClient {
 public:
  KVClient(std::shared_ptr<Channel> channel)
      : stub_(KVService::NewStub(channel)),
        qid_(0) {
  }

  int get(const std::string& key, std::string* value) {
    ClientContext context;
    TINYKV::ReqKey req_key;
    req_key.set_qid(qid_++);
    req_key.add_key(key);
    TINYKV::RespValue rsp;
    Status status = stub_->Get(&context, req_key, &rsp);
    if (!status.ok()) {
        fprintf(stderr, "kv get [%s] failed.\n", key.c_str());
        return -1;
    }
    *value = rsp.value(0);
    return 0;
  }

  int insert(const std::string& key, const std::string& value) {
    ClientContext context;
    TINYKV::ReqKeyValue req_key_value;
    req_key_value.set_qid(qid_++);
    req_key_value.add_key(key);
    req_key_value.add_value(value);
    TINYKV::RespValue rsp_value;
    Status status = stub_->Insert(&context, req_key_value, &rsp_value);
    if (!status.ok()) {
        fprintf(stderr, "kv insert [%s] [%s] failed.\n", key.c_str(), value.c_str());
        return -1;
    }
    return 0;
  }

 private:
  std::unique_ptr<KVService::Stub> stub_;
  int64_t qid_;
};

int main(int argc, char** argv) {
  KVClient kv_client(
      grpc::CreateChannel("localhost:50051",
                          grpc::InsecureChannelCredentials()));
  kv_client.insert("abc", "def");
  kv_client.insert("wangmeng12", "ttymy");
  std::string value;
  kv_client.get("abc", &value);
  assert(value == "def");
  kv_client.get("wangmeng12", &value);
  assert(value == "ttymy");
  return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=80: */
