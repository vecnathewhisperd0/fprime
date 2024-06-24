#include "Os/Mutex.hpp"

#ifndef OS_STUB_MUTEX_TEST_HPP
#define OS_STUB_MUTEX_TEST_HPP
namespace Os {
namespace Stub {
namespace Mutex {
namespace Test {

//! Data that supports the stubbed Mutex implementation.
//!/
struct StaticData {
    //! Enumeration of last function called
    //!
    enum LastFn {
        NONE_FN,
        CONSTRUCT_FN,
        DESTRUCT_FN,
        TAKE_FN,
        RELEASE_FN,
        LOCK_FN,
        UNLOCK_FN,
        GETHANDLE_FN
    };
    StaticData() = default;
    ~StaticData() = default;

    //! Last function called
    LastFn lastCalled = NONE_FN;

    Os::Mutex::Status takeStatus = Os::Mutex::Status::OP_OK;
    Os::Mutex::Status releaseStatus = Os::Mutex::Status::OP_OK;

    // Singleton data
    static StaticData data;
};

//! Test task handle
class TestMutexHandle : public MutexHandle {};

//! Implementation of task
class TestMutex : public MutexInterface {
  public:
    //! Constructor
    TestMutex();

    //! Destructor
    ~TestMutex() override;

    //! \brief lock mutex and assert on status
    void lock() override;

    //! \brief unlock mutex and assert on status
    void unLock() override;

    //! \brief lock mutex and return status
    Status take() override;

    //! \brief unlock mutex and return status
    Status release() override;

    //! \brief return the underlying mutex handle (implementation specific)
    //! \return internal task handle representation
    MutexHandle* getHandle() override;

};

}
}
}
}
#endif // End OS_STUB_MUTEX_TEST_HPP
