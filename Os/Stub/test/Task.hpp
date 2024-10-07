#include "Os/Task.hpp"

#ifndef OS_STUB_TASK_TEST_HPP
#define OS_STUB_TASK_TEST_HPP
namespace Os {
namespace Stub {
namespace Task {
namespace Test {

//! Data that supports the stubbed File implementation.
//!/
struct StaticData {
    //! Enumeration of last function called
    //!
    enum LastFn {
        NONE_FN,
        CONSTRUCT_FN,
        DESTRUCT_FN,
        DELAY_FN,
        JOIN_FN,
        SUSPEND_FN,
        RESUME_FN,
        START_FN
    };
    StaticData() = default;
    ~StaticData() = default;

    //! Last function called
    LastFn lastCalled = NONE_FN;

    Os::Task::SuspensionType suspensionType;
    Fw::TimeInterval delay;

    Os::Task::Status delayStatus = Os::Task::Status::OP_OK;
    Os::Task::Status joinStatus = Os::Task::Status::OP_OK;
    Os::Task::Status startStatus = Os::Task::Status::OP_OK;

    // Singleton data
    static StaticData data;
};

//! Test task handle
class TestTaskHandle : public TaskHandle {};

//! Implementation of task
class TestTask : public TaskInterface {
  public:
    //! Constructor
    TestTask();

    //! Destructor
    ~TestTask() override;

    //! \brief perform required task start actions
    void onStart() override;

    //! \brief block until the task has ended
    //!
    //! Blocks the current (calling) task until this task execution has ended. Callers should ensure that any
    //! signals required to stop this task have already been emitted or will be emitted by another task.
    //!
    //! \return status of the block
    Status join() override;

    //! \brief suspend the task given the suspension type
    //!
    //! Suspends the task. Some implementations track if the suspension of a task was intentional or
    //! unintentional. The supplied `suspensionType` parameter indicates that this was intentional or
    //! unintentional. The type of suspension is also returned when calling `isSuspended`.
    //!
    //! \param suspensionType intentionality of the suspension
    void suspend(SuspensionType suspensionType) override;

    //! \brief resume a suspended task
    //!
    //! Resumes this task. Not started, running, and exited tasks take no action.
    //!
    void resume() override;

    //! \brief return the underlying task handle (implementation specific)
    //! \return internal task handle representation
    TaskHandle* getHandle() override;

    //! \brief determine if the task requires cooperative multitasking
    //!
    //! Some task implementations require cooperative multitasking where the task execution is run by a user
    //! defined task scheduler and not the operating system task scheduler. These tasks cooperatively on
    //! multitask by doing one unit of work and return from the function.
    //!
    //! This function indicates if the task requires cooperative support.
    //! The default implementation returns false.
    //!
    //! \return true when the task expects cooperation, false otherwise
    bool isCooperative() override;

    //! \brief start the task
    //!
    //! Starts the task given the supplied arguments.
    //!
    //! \param arguments: arguments supplied to the task start call
    //! \return status of the task start
    Status start(const Arguments& arguments) override;
};

}
}
}
}
#endif // End OS_STUB_TASK_TEST_HPP
