#ifdef USING_BRPC
#include "butil/logging.h" 
#else
#include <glog/logging.h>
#endif
#include <gflags/gflags.h>

extern int RunServer(); 
int main(int argc, char** argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);
#ifndef USING_BRPC
    google::InitGoogleLogging(argv[0]);
#endif
    RunServer();
    return 0;
}
