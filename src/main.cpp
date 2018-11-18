#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include "grpcpp/grpcpp.h"
#include "tiny_kv.h" 
#include "plain_kv_engine.h" 
#include "interface/service.grpc.pb.h"
#include "interface/service.pb.h"

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
          fprintf(stderr, "Insert cancelled for key value size not match [%d/%d]\n", key_size, value_size);
          return Status::CANCELLED;
      }
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

  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}
