// Copyright 2017 codevil All Rights Reserved.
// Author: codevil.w.m@gmail.com (M. W.)
#include <random>
#include <iostream>
int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 6);
    for(int n=0; n<10; ++n)
      std::cout << dis(gen) << ' ';
    std::cout << '\n';
    return 0;
}

/* vim: set expandtab ts=2 sw=2 sts=2 tw=80: */
