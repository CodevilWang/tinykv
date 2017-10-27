#include <stdio.h>
#include <string>
#include <unordered_map>
namespace TinyKV {
class PlainEngine {
    public:
        int insert(const std::string& key, const std::string& value) {
            _map.insert(std::make_pair(key, value));
            return 0;
        }
        int insert(const std::pair<std::string, std::string>& kv) {
            _map.insert(kv);
            return 0;
        }
        bool has_key(const std::string& key) {
            if (_map.find(key) != _map.end()) {
                return true;
            }
            return false;
        } 
        std::string operator[] (const std::string& key) {
            return _map[key];
        } 
    private:
        std::unordered_map<std::string, std::string> _map;

};
}  // namespace TinyKV
