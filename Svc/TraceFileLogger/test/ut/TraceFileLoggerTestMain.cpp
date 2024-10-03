// ======================================================================
// \title  TraceFileLoggerTestMain.cpp
// \author sreddy
// \brief  cpp file for TraceFileLogger component test main function
// ======================================================================

#include "TraceFileLoggerTester.hpp"

TEST(Nominal, test_startup) {
    Svc::TraceFileLoggerTester tester;
    tester.test_startup();
}

TEST(Nominal, test_log_file) {
    Svc::TraceFileLoggerTester tester;
    tester.test_startup();
    tester.test_file();
}

TEST(Nominal, test_filter_trace_type) {
    Svc::TraceFileLoggerTester tester;
    tester.test_startup();
    tester.test_filter_trace_type();
}

TEST(Nominal, test_filter_trace_id) {
    Svc::TraceFileLoggerTester tester;
    tester.test_startup();
    tester.test_filter_trace_id();
    tester.test_startup();
    tester.test_trace_enable();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
