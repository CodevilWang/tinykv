// Copyright 2017 codevil All Rights Reserved.
// Author: codevil.w.m@gmail.com (M. W.)

#include <stdio.h>
#include <random>
#include <vector>

int main() {
    int key_len = 0;
    int value_len = 0;
    std::vector<char>  char_pool;
    for (int i = 0; i < 10; ++i) {
        char_pool.push_back('0' + i);
    }
    for (int i = 0; i < 26; ++i) {
        char_pool.push_back('a' + i);
    }
    for (int i = 0; i < 26; ++i) {
        char_pool.push_back('A' + i);
    }
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(char_pool.begin(), char_pool.end(), std::default_random_engine(seed));
    scanf("%d", &key_len);
    scanf("%d", &value_len);
    scanf("%d", &total_len);
    if (key_len == 0) {
        printf("key length must not be 0.\n");
        return -1;
    }
    std::ofstream ofs("kv.dat");
    for (int i = 0; i < total_len; ++i) {
        std::string key;
        std::string value;
        gen_rand_str(char_pool, key_len, &key);
        gen_rand_str(char_pool, value_len, &value);

    }

    return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=80: */
