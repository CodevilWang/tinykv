#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include "tiny_kv.h" 
#include "plain_kv_engine.h" 

int main() {
    TINYKV::TinyKV<TINYKV::PlainEngine> kv;
    kv.insert("abc", "def");
    kv.insert("xyz", "cc");
    std::string value;
    kv.get("abc", &value);
    printf("%s\n", value.c_str());
    kv.get("xyz", &value);
    printf("%s\n", value.c_str());
    return 0;
}
