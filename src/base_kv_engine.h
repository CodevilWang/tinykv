// Copyright 2018 All Rights Reserved.
// Author: codevil.w.m@gmail.com (M W)
#ifndef  TINY_KV_SRC_BASE_KV_ENGINE_H_
#define  TINY_KV_SRC_BASE_KV_ENGINE_H_
#include <stdio.h>
#include <string>
#include <unordered_map>
namespace TINYKV {
class BaseKVEngine {
    public:
        BaseKVEngine() {}
        virtual ~BaseKVEngine() {}
        virtual bool init() = 0;
        virtual int insert(const std::string& key, const std::string& value) = 0;
        virtual int insert(const std::pair<std::string, std::string>& kv) = 0;
        virtual bool has_key(const std::string& key) = 0;
        virtual std::string operator[] (const std::string& key)  = 0;
};
}  // namespace TINYKV

#endif  // BASE_KV_ENGINE_H_
/* vim: set expandtab ts=2 sw=2 sts=2 tw=80: */
