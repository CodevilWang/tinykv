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
#else
    logging::LoggingSettings ls;
    ls.logging_dest = logging::LOG_TO_FILE;
    logging::InitLogging(ls);
#endif
    RunServer();
    return 0;
}
