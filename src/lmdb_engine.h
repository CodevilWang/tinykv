#ifndef  TINY_KV_SRC_LMDB_ENGINE_H_
#define  TINY_KV_SRC_LMB_ENGINE_H_
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <sstream>
#include <functional>
#include "base_kv_engine.h" 
#include "lmdb.h"
#include "gflags/gflags.h"
DEFINE_string(db_dir, "./db", "lmdb local dir");
namespace TINYKV {
class LMDBEngine : public BaseKVEngine {
    public:
        LMDBEngine() {
        }

        bool init() {
		    if (mdb_env_create(&env) != MDB_SUCCESS) {
                LOG(ERROR) << "MDB_ENV_CREATE failed"; 
                return false;
            }
		    if (mdb_env_set_maxreaders(env, 1) != MDB_SUCCESS) {
                LOG(ERROR) << "MDB_SET_MAXREADERS failed"; 
                return false;
            }
		    if (mdb_env_set_mapsize(env, 10485760) != MDB_SUCCESS) {
                LOG(ERROR) << "MDB_SET_MAPSIZE failed"; 
                return false;
            }
            if (mdb_env_open(env, FLAGS_db_dir.c_str(), MDB_FIXEDMAP /*|MDB_NOSYNC*/, 0664) != MDB_SUCCESS) {
                LOG(ERROR) << "MDB_ENV_OPEN failed"; 
                return false;
            }
            return true;
        }

        int insert(const std::string& key, const std::string& value) {
	        MDB_val i_key, i_value;
            i_key.mv_size = key.length();
            i_key.mv_data = const_cast<char*>(key.c_str());
            i_value.mv_size = value.length();
            i_value.mv_data = const_cast<char*>(value.c_str());
	        MDB_txn *txn;
	        MDB_dbi dbi;
		    mdb_txn_begin(env, NULL, 0, &txn);
		    mdb_dbi_open(txn, NULL, 0, &dbi);
            int ret = mdb_put(txn, dbi, &i_key, &i_value, MDB_NOOVERWRITE);
            if (ret == 0) {
                // TODO(check)
		        mdb_txn_commit(txn);
                return true;
            }
            // TODO(check)
		    mdb_txn_commit(txn);
            LOG(ERROR) << "insert failed ret " << ret;
            return false;
        }
        int insert(const std::pair<std::string, std::string>& kv) {
            return insert(kv.first, kv.second);
        }
        bool has_key(const std::string& key) {
            return true;
        }
        std::string operator[] (const std::string& key) {
            return "";
        }
    private:
        MDB_env* env;
};
}  // namespace TINYKV
#endif
