// Copyright 2018 All Rights Reserved.
// Author: codevil.w.m@gmail.com (M W)
#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
template<class K, class V>
class BaseClass {
    public:
        BaseClass() {}
        virtual ~BaseClass() {}
        virtual void insert(const K& key, const V& value) {
            _map.insert(std::make_pair(key, value));
        }
        virtual int get(const K& key, V* value) {
            typename std::unordered_map<K, V>::const_iterator c_iter = _map.find(key);
            if (c_iter != _map.end()) {
                *value = c_iter->second;
            }
            return 0;
        }
    protected:
        std::unordered_map<K, V> _map;

};

template<class K, class V>
class DerivedClass : public BaseClass<K, V> {
    public:
        DerivedClass() {}
        virtual ~DerivedClass() {}
        virtual int get(const K& key, V* value) {
            printf("in derived get [%u]\n", _map.size());
            typename std::map<K, V>::const_iterator c_iter = _map.find(key);
            if (c_iter != _map.end()) {
                *value = c_iter->second;
            }
            return 0;
        }
    protected:
        std::map<K, V> _map;
};

template<class Imp>
class Container {
public:
    Container() {
        _inter = new Imp();
    }
    virtual ~Container() {}
    void work() {
        std::string value;
        int ret = _inter->get("abc", &value);
        printf("return value is %d\n", ret);
    }
private:
    Imp* _inter;
};

int main() {
    BaseClass<int, int> bmap;
    bmap.insert(10, 10);
    int value = -1;
    bmap.get(10, &value);
    printf("%d\n", value);
    DerivedClass<int, int> dmap;
    dmap.insert(10, 11);
    dmap.insert(11, 11);
    dmap.get(10, &value);
    printf("%d\n", value);
    Container<BaseClass<std::string, std::string>> c;
    c.work();
    // Container* d = new Container<DerivedClass<std::string, std::string>>();
    // d->work();
    return 0;
}
/* vim: set expandtab ts=2 sw=2 sts=2 tw=80: */
