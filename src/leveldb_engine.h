#ifndef  TINY_KV_SRC_LEVELDB_ENGINE_H_
#define  TINY_KV_SRC_LEVELDB_ENGINE_H_
#include <stdio.h>
#include <string>
#include <unordered_map>
#include "base_kv_engine.h" 
#include "leveldb/db.h"
#include "gflags/gflags.h"
DEFINE_string(leveldb_dir, "./db", "leveldb local dir");
namespace TINYKV {
class LevelDBEngine : public BaseKVEngine {
    public:
        LevelDBEngine() {
        }
        bool init() {
            leveldb::Options options;
            options.create_if_missing = true;
            leveldb::DB* db;
            leveldb::Status status = leveldb::DB::Open(options, FLAGS_leveldb_dir, &db);
            if (!status.ok()) {
                LOG(ERROR) << "open leveldb [" << FLAGS_leveldb_dir << "] failed.";
                return false;
            }
            _db.reset(db);
            return true;
        }
        int insert(const std::string& key, const std::string& value) {
            // TODO(use template to check)
            leveldb::Status status = _db->Put(leveldb::WriteOptions(), key, value);
            if (!status.ok()) {
                 LOG(ERROR) << "Put " << key << " " << value << " failed.";
                 return 1;
            }
            return 0;
        }
        int insert(const std::pair<std::string, std::string>& kv) {
            leveldb::Status status = _db->Put(leveldb::WriteOptions(), kv.first, kv.second);
            if (!status.ok()) {
                 LOG(ERROR) << "Put " << kv.first << " " << kv.second << " failed.";
                 return 1;
            }
            return 0;
        }
        bool has_key(const std::string& key) {
            std::string value;
            leveldb::Status status = _db->Get(leveldb::ReadOptions(), key, &value);
            if (status.ok()) {
                return true;
            }
            if (status.IsNotFound()) {
                return false;
            }
            LOG(ERROR) << "some error when has_key [" << key << "]";
            return false;
        }
        std::string operator[] (const std::string& key) {
            std::string value;
            leveldb::Status status = _db->Get(leveldb::ReadOptions(), key, &value);
            if (!status.ok()) {
                LOG(ERROR) << "some error when Get [" << key << "]";
                return "";
            }
            return value;
        }
    private:
        std::unique_ptr<leveldb::DB> _db;
};
}  // namespace TINYKV
#endif
