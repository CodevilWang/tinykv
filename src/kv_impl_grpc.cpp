// Copyright 2018 All Rights Reserved.
// Author: codevil.w.m@gmail.com (M W)

#ifdef USING_GRPC
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include "tiny_kv.h" 
#include "plain_kv_engine.h" 
#include "interface/service.pb.h"
#include "grpcpp/grpcpp.h"
#include "interface/service.grpc.pb.h"
#include <glog/logging.h>
using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using TINYKV::ReqKey;
using TINYKV::ReqKeyValue;
using TINYKV::RespValue;

// Logic and data behind the server's behavior.
template <class KVEngine>
class KVServiceImpl final : public TINYKV::KVService::Service {
public:
  KVServiceImpl() {
      _kv_engine = new KVEngine();
  }
  virtual ~KVServiceImpl() {}
  int init() {
      if (_kv_engine->init() != 0) {
         return -1;
      }
      return 0;
  }
  Status Get(ServerContext* context, const ReqKey* request,
                  RespValue* reply) override {
    if (request->key_size() == 0) {
        LOG(ERROR) << "Reqid=" << request->qid() << " miss request key.";
        return Status::OK;
    }
    LOG(INFO) << "Reqid=" << request->qid() << " key_size=" << request->key_size()
              << " Sample key=" << request->key(0);
    reply->clear_value();
    for (int i = 0; i < request->key_size(); ++i) {
        std::string value;
        if (_kv_engine->get(request->key(i), &value) == 0 && value.length() > 0) {
            reply->add_value(value);
        }
    }
    return Status::OK;
  }
  Status Insert(ServerContext* context, const ReqKeyValue* req_key_value,
                 RespValue* relay) override {
      int key_size = req_key_value->key_size();
      int value_size = req_key_value->value_size();
      if (key_size != value_size) {
          LOG(WARNING) << "Insert cancelled for key value size not match [" << key_size << "/" << value_size << "]";
          return Status::CANCELLED;
      }
      LOG(INFO) << "Reqid=" << req_key_value->qid() << " key_size=" << req_key_value->key_size()
                << " Sample key=" << req_key_value->key(0);
      for (int i = 0; i < key_size; ++i) {
          _kv_engine->insert(req_key_value->key(i), req_key_value->value(i));
      }
      return Status::OK;
  }
private:
  KVEngine* _kv_engine;
};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  KVServiceImpl<TINYKV::TinyKV<TINYKV::PlainEngine>> service;
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  LOG(INFO) << "Server listening on " << server_address;
  server->Wait();
}
#endif

/* vim: set expandtab ts=2 sw=2 sts=2 tw=80: */
