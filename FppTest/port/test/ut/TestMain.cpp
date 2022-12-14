// ----------------------------------------------------------------------
// TestMain.cpp
// ----------------------------------------------------------------------

#include "Tester.hpp"
#include "FppTest/port/PortTypes.hpp"
#include "FppTest/typed_tests/PortTest.hpp"

#include "gtest/gtest.h"

// Typed port tests
using TypedPortTestImplementations = ::testing::Types<
    FppTest::Port::NoArgsPort,
    FppTest::Port::PrimitiveArgsPort,
    FppTest::Port::StringArgsPort,
    FppTest::Port::EnumArgsPort,
    FppTest::Port::ArrayArgsPort,
    FppTest::Port::StructArgsPort,
    FppTest::Port::NoArgsReturnPort,
    FppTest::Port::PrimitiveReturnPort,
    FppTest::Port::EnumReturnPort,
    FppTest::Port::ArrayReturnPort,
    FppTest::Port::StructReturnPort
>;

INSTANTIATE_TYPED_TEST_SUITE_P(FppTest,
                               TypedPortTest,
                               TypedPortTestImplementations);

// Serial port tests
using SerialPortTestImplementations = ::testing::Types<
    FppTest::Port::NoArgsPort,
    FppTest::Port::PrimitiveArgsPort,
    FppTest::Port::StringArgsPort,
    FppTest::Port::EnumArgsPort,
    FppTest::Port::ArrayArgsPort,
    FppTest::Port::StructArgsPort,
    FppTest::Port::SerialArgsPort
>;

INSTANTIATE_TYPED_TEST_SUITE_P(FppTest,
                               SerialPortTest,
                               SerialPortTestImplementations);

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
