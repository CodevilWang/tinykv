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
DECLARE_string(db_dir);
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
	    if (mdb_env_set_maxreaders(env, 10) != MDB_SUCCESS) {
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
            return 0;
        }
        // TODO(check)
	    mdb_txn_commit(txn);
        LOG(ERROR) << "insert failed ret " << ret;
        return -1;
    }
    int insert(const std::pair<std::string, std::string>& kv) {
        return insert(kv.first, kv.second);
    }
    bool has_key(const std::string& key) {
        return operator[](key) != "";
    }
    std::string operator[] (const std::string& key) {
        MDB_txn *txn;
        MDB_dbi dbi;
	    // MDB_cursor *cursor;
        static const std::string ERR_STR = "";
        int rc = 0;
        rc = mdb_txn_begin(env, NULL, MDB_RDONLY, &txn);
		if (rc != 0) {
            LOG(ERROR) << "mdb_txn_begin failed rc=" << rc;
            fprintf(stderr, "first \n");
            return ERR_STR;
        }
		if (mdb_dbi_open(txn, NULL, 0, &dbi) != 0) {
            fprintf(stderr, "second \n");
            LOG(ERROR) << "mdb_dbi_open failed.";
            return ERR_STR;
        }
        MDB_val i_key, i_data;
        i_key.mv_size = key.length();
        i_key.mv_data = const_cast<char*>(key.c_str());
        rc = mdb_get(txn, dbi, &i_key, &i_data);
		// mdb_cursor_close(cursor);
        std::string ret_str;
        if (rc == 0) {
            ret_str =  std::string((char*)i_data.mv_data, i_data.mv_size);
            fprintf(stderr, "succ %s\n", ret_str.c_str()); 
        } else {
            ret_str = ERR_STR;
        }
		mdb_txn_commit(txn);
        return ret_str; 
    }
private:
    MDB_env* env;
};
}  // namespace TINYKV
#endif
