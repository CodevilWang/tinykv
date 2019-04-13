#include <glog/logging.h>
#include <gflags/gflags.h>

extern void RunServer(); 
int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    google::InitGoogleLogging(argv[0]);
    RunServer();
    return 0;
}
