// Copyright 2017 codevil All Rights Reserved.
// Author: codevil.w.m@gmail.com (M. W.)
#include <stdio.h>
#include <vector>
#include <random>
#include <chrono>

int main() {
    std::vector<int> data = {1, 2, 3, 4,5};
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(data.begin(), data.end(), std::default_random_engine(seed));
    for (auto i : data) {
        printf("%d\n", i);
    }
    return 0;
}




















/* vim: set expandtab ts=2 sw=2 sts=2 tw=80: */
