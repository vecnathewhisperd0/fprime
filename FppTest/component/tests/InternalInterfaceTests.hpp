#include "test/ut/Tester.hpp"

// ----------------------------------------------------------------------
// Internal interface test declarations
// ----------------------------------------------------------------------

#define INTERNAL_INT_TEST_DECL(TYPE) void testInternalInterface(FppTest::Types::TYPE& data);

#define INTERNAL_INT_TEST_DECLS                           \
    INTERNAL_INT_TEST_DECL(NoParams)                      \
    INTERNAL_INT_TEST_DECL(PrimitiveParams)               \
    INTERNAL_INT_TEST_DECL(InternalInterfaceStringParams) \
    INTERNAL_INT_TEST_DECL(EnumParam)                     \
    INTERNAL_INT_TEST_DECL(ArrayParam)                    \
    INTERNAL_INT_TEST_DECL(StructParam)
