// ======================================================================
// \title  StreamCrossover.hpp
// \author ethanchee
// \brief  cpp file for StreamCrossover test harness implementation class
// ======================================================================

#include "Tester.hpp"

namespace Drv {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  Tester ::
    Tester() :
      StreamCrossoverGTestBase("Tester", Tester::MAX_HISTORY_SIZE),
      component("StreamCrossover")
  {
    this->initComponents();
    this->connectPorts();
  }

  Tester ::
    ~Tester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void Tester ::
    sendTestBuffer()
  {
    this->clearHistory();

    U8 testStr[6] = "test\n";
    Fw::Buffer sendBuffer(testStr, sizeof(testStr));
    this->invoke_to_streamIn(0, sendBuffer, Drv::RecvStatus::RECV_OK);

    // Ensure only one buffer was sent to streamOut
    ASSERT_from_streamOut_SIZE(1);

    // Ensure the sendBuffer was sent
    ASSERT_from_streamOut(0, sendBuffer);
  }

  // ----------------------------------------------------------------------
  // Handlers for typed from ports
  // ----------------------------------------------------------------------

  Drv::SendStatus Tester ::
    from_streamOut_handler(
        const NATIVE_INT_TYPE portNum,
        Fw::Buffer &sendBuffer
    )
  {
    this->pushFromPortEntry_streamOut(sendBuffer);

    U8 testStr[6] = "test\n";
    Fw::Buffer cmpBuffer(testStr, sizeof(testStr));

    if(!(cmpBuffer == sendBuffer))
    {
      return Drv::SendStatus::SEND_ERROR;
    }

    return Drv::SendStatus::SEND_OK;
  }


} // end namespace Drv
