// Copyright 2018 All Rights Reserved.
// Author: codevil.w.m@gmail.com (M W)

#ifdef USING_BRPC
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include "tiny_kv.h" 
#include "plain_kv_engine.h" 
#include "interface/service_brpc.pb.h"
#include "butil/logging.h"
#include <brpc/server.h>
#include "leveldb_engine.h" 
#include "lmdb_engine.h" 
// using TINYKV::ReqKey;
// using TINYKV::ReqKeyValue;
// using TINYKV::RespValue;

// Logic and data behind the server's behavior.
DEFINE_string(db_dir, "./db", "local db dir");
DEFINE_string(db_type, "plain", "local db engine type");
template <class KVEngine>
class KVServiceImpl final : public TINYKV::KVService {
public:
  KVServiceImpl() {
      _kv_engine = new KVEngine();
  }
  virtual ~KVServiceImpl() {}
  bool init() {
      return _kv_engine->init();
  }
  virtual void Get(::google::protobuf::RpcController* controller,
                       const ::TINYKV::ReqKey* request,
                       ::TINYKV::RespValue* response,
                       ::google::protobuf::Closure* done) {
    brpc::ClosureGuard done_guard(done);
    if (request->key_size() == 0) {
        LOG(ERROR) << "Reqid=" << request->qid() << " miss request key.";
        response->set_status(1);
        return;
    }
    LOG(INFO) << "Reqid=" << request->qid() << " key_size=" << request->key_size()
              << " Sample key=" << request->key(0);
    response->clear_value();
    for (int i = 0; i < request->key_size(); ++i) {
        std::string value;
        if (_kv_engine->get(request->key(i), &value) == 0 && value.length() > 0) {
            response->add_value(value);
        }
    }
    response->set_status(0);
    return;
  }
  virtual void Insert(::google::protobuf::RpcController* controller,
                       const ::TINYKV::ReqKeyValue* request,
                       ::TINYKV::RespValue* response,
                       ::google::protobuf::Closure* done) {
      brpc::ClosureGuard done_guard(done);
      int key_size = request->key_size();
      int value_size = request->value_size();
      if (key_size != value_size) {
          LOG(WARNING) << "Insert cancelled for key value size not match [" << key_size << "/" << value_size << "]";
          response->set_status(1);
          return;
      }
      LOG(INFO) << "Reqid=" << request->qid() << " key_size=" << request->key_size()
                << " Sample key=" << request->key(0);
      for (int i = 0; i < key_size; ++i) {
          _kv_engine->insert(request->key(i), request->value(i));
      }
      response->set_status(0);
      return;
  }
private:
  KVEngine* _kv_engine;
};

#define SERVICE_IMPL_PTR(ENGINE_TYPE) \
  do { \
      auto service_impl_ptr = new(std::nothrow)KVServiceImpl<TINYKV::TinyKV<ENGINE_TYPE>>(); \
      if (!service_impl_ptr->init()) { \
          LOG(WARNING) << "service_impl_ptr init failed."; \
          return -1; \
      } \
      service.reset(service_impl_ptr); \
  } while(0);

int RunServer() {
    // std::string server_address("0.0.0.0:50051");
    brpc::Server server;
    // KVServiceImpl<TINYKV::TinyKV<TINYKV::LevelDBEngine>> service;
    std::unique_ptr<TINYKV::KVService> service(nullptr); 
    if (FLAGS_db_type == "plain") {
        SERVICE_IMPL_PTR(TINYKV::PlainEngine);
    }

    if (FLAGS_db_type == "lmdb") {
        // service.reset(new(std::nothrow)KVServiceImpl<TINYKV::TinyKV<TINYKV::LevelDBEngine>>());
        SERVICE_IMPL_PTR(TINYKV::LMDBEngine);
    }

    if (FLAGS_db_type == "leveldb") {
        SERVICE_IMPL_PTR(TINYKV::LevelDBEngine);
    }

    // KVServiceImpl<TINYKV::TinyKV<TINYKV::LMDBEngine>> service;
    // KVServiceImpl<TINYKV::TinyKV<TINYKV::PlainEngine>> service;
    // if (!service->init()) {
    //     LOG(WARNING) << "KVServiceImpl init failed.";
    //     return -1;
    // }
    if (server.AddService(service.get(),
                    brpc::SERVER_DOESNT_OWN_SERVICE) != 0) {
        LOG(ERROR) << "Fail to add service";
        return -1;
    }
    brpc::ServerOptions options;
    // options.idle_timeout_sec = FLAGS_idle_timeout_s;
    if (server.Start(50051, &options) != 0) {
        LOG(ERROR) << "Fail to start Server";
        return -1;
    }
    LOG(INFO) << "Server listening on " << 50051; 
    server.RunUntilAskedToQuit();
    return 0;
}
#endif

/* vim: set expandtab ts=2 sw=2 sts=2 tw=80: */
