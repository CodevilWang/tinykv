// Copyright 2018 All Rights Reserved.
// Author: codevil.w.m@gmail.com (M W)

#include <stdio.h>
#include <assert.h>
#include <iostream>
#include <fstream>
#include "src/interface/service.pb.h" 
int main() {
    std::ofstream ofs;
    ofs.open("out.dat", std::ios::out | std::ios::binary);
    if (!ofs.is_open()) {
        printf("open failed.\n");
    }
    {
        TINYKV::ReqKey rkey;
        rkey.set_qid(10);
        rkey.add_key("value max");
        if (!rkey.SerializeToOstream(&ofs)) {
            printf("Serialize To String failed.\n");
            return -1;
        }
    }
    {
        TINYKV::ReqKey rkey;
        rkey.set_qid(11);
        rkey.add_key("value max2");
        if (!rkey.SerializeToOstream(&ofs)) {
            printf("Serialize To String failed.\n");
            return -1;
        }
    }

    ofs.close();
    std::ifstream ifs;
    ifs.open("out.dat", std::ios::in | std::ios::binary);
    if (!ifs.is_open()) {
        printf("open out.dat for read failed.\n");
        return -1;
    }
    {
        TINYKV::ReqKey in_key;
        in_key.ParseFromIstream(&ifs);
        // assert(in_key.qid() == 10);
        // assert(in_key.key(0) == "value max");
        printf("%lld\t%d\n", in_key.qid(), in_key.key_size());
        printf("%s\n", in_key.key(0).c_str());
    }

    return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=80: */
