#include <sstream>
#include <fstream>
#include <gflags/gflags.h>
#ifdef USING_BRPC
#include "butil/logging.h" 
#else
#include <glog/logging.h>
#endif
#include "brpc/channel.h"
#include "service_brpc.pb.h"

DEFINE_string(kvsvc_host, "", "kv service host info");
DEFINE_string(load_balancer, "rr", "The algorithm for load balancing");
DEFINE_int32(kvsvc_timeout_ms, 1000, "http server timeout millisecond");
DEFINE_int32(kvsvc_retry_times, 3, "max retry time where call feed clarity http server failed");
DEFINE_int32(kvsvc_retry_interval, 300, "time interval us between two retry call");
DEFINE_int32(kvsvc_backup_request_interval, 800, "backup request interval");

class KVClient {
public:
    KVClient() : _qid(0) {}
    virtual ~KVClient() {}
    bool init() {
        brpc::ChannelOptions options;
        options.protocol = brpc::PROTOCOL_BAIDU_STD;
        options.timeout_ms = FLAGS_kvsvc_timeout_ms/*milliseconds*/;
        options.max_retry = FLAGS_kvsvc_retry_times;
        options.backup_request_ms = FLAGS_kvsvc_backup_request_interval;
        if (0 != _server_channel.Init(FLAGS_kvsvc_host.c_str(), FLAGS_load_balancer.c_str(),
                                           &options)) {
            LOG(WARNING) << "init server channel failed!";
            return false;
        }
        return true;
    }
    int get(const std::string& key) {
        TINYKV::ReqKey kv_req;
        TINYKV::RespValue kv_res;
        kv_req.set_qid(_qid++);
        kv_req.add_key(key);
        TINYKV::KVService::Stub stub(&_server_channel);
        brpc::Controller cntl;
        stub.Get(&cntl, &kv_req, &kv_res, nullptr);
        if (cntl.Failed()) {
            LOG(ERROR) << "Get Key[" << key << "] failed [" << cntl.ErrorText() << "]";
            return 1;
        }
        LOG(INFO) << "Get Key[" << key << "] value[" << kv_res.value(0) << "]";
        return 0;
    }

    int insert(const std::string& key, const std::string& value) {
      TINYKV::ReqKeyValue req_key_value;
      req_key_value.set_qid(_qid++);
      req_key_value.add_key(key);
      req_key_value.add_value(value);
      TINYKV::RespValue kv_rsp;
      TINYKV::KVService::Stub stub(&_server_channel);
      brpc::Controller cntl;
      stub.Insert(&cntl, &req_key_value, &kv_rsp, nullptr);
      if (cntl.Failed()) {
          LOG(ERROR) << "kv insert [" << key << "] [" << value << "] failed [" << cntl.ErrorText() << "]";
          return -1;
      }
      LOG(INFO) << "Insert Key[" << key << "] value[" << value << "]";
      return 0;
    }
private:
    brpc::Channel _server_channel;
    int64_t _qid;

};

static const char* KEY_LIST_FILE = "out_key.txt";
static const char* VALUE_LIST_FILE = "out_value.txt";

int load_key_value_list(std::vector<std::string>* keys, std::vector<std::string>* values) {
    std::ifstream key_list_fd;
    key_list_fd.open(KEY_LIST_FILE, std::ifstream::in);
    if (not key_list_fd.is_open()) {
        LOG(ERROR) << "open key_list_file " << KEY_LIST_FILE << " failed";
        return -1;
    }
    std::ifstream value_list_fd;
    value_list_fd.open(VALUE_LIST_FILE, std::ifstream::in);
    if (not value_list_fd.is_open()) {
        LOG(ERROR) << "open value_list_file " << VALUE_LIST_FILE << " failed";
        return -1;
    }
    while (key_list_fd.good()) {
        std::string tmp_key;
        std::getline(key_list_fd, tmp_key);
        if (tmp_key.length() > 0) {
            (*keys).push_back(tmp_key);
        }
    }
    while (value_list_fd.good()) {
        std::string tmp_value;
        std::getline(value_list_fd, tmp_value);
        if (tmp_value.length() > 0) {
            (*values).push_back(tmp_value);
        }
    }
    assert(keys->size() == values->size());
    LOG(INFO) << "Loaded " << keys->size() << " keys & " << values->size() << " values";
    return 0;
}


int main(int argc, char** argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);
#ifndef USING_BRPC
    google::InitGoogleLogging(argv[0]);
#endif
    KVClient kv_client;
    if (!kv_client.init()) {
        LOG(ERROR) << "init kvclient failed.";
        return -1;
    }
    std::vector<std::string> keys;
    std::vector<std::string> values;
    if (load_key_value_list(&keys, &values) != 0) {
        LOG(ERROR) << "Load key_value_list failed.";
        return -1;
    }
    // single thread insert
    for (int i = 0; i < keys.size(); ++i) {
      kv_client.insert(keys[i], values[i]);
      // kv_client.get(keys[i]);
    }
    return 0;
}

/* vim: set expandtab ts=4 sw=4 sts=4 tw=100: */
