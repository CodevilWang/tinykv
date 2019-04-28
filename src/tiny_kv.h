#ifndef  TINY_KV_SRC_TINY_KV_H_
#define  TINY_KV_SRC_TINY_KV_H_
#include <string>
namespace TINYKV {
// support multi KVEngine
template<class T>
class TinyKV {
    public:
        TinyKV() {
            _imp = new(std::nothrow) KVEngine();
            if (!_imp) {
                throw std::runtime_error("new KVEngine fail.");
            }
        }; 
        ~TinyKV() = default;
        bool init() {
            return _imp->init();
        }
        int insert(const std::string& key, const std::string& value) {
            _imp->insert(std::make_pair(key, value));
            return 0;
        }
        int insert(const std::pair<std::string, std::string>& pair) {
            _imp->insert(pair);
            return 0;
        }
        int get(const std::string& key, std::string* value) {
            if (!_imp->has_key(key)) {
                return -1;
            }
            *value = (*_imp)[key];
            return 0;
        }
    private:
        typedef T KVEngine;
        KVEngine* _imp;
};
};  // namespace TINYKV

#endif  // TINY_KV_SRC_TINY_KV_H_
