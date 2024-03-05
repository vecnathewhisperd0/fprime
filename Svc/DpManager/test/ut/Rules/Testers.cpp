// ======================================================================
// \title  Testers.cpp
// \author Rob Bocchino
// \brief  Testers class implementation
//
// \copyright
// Copyright (C) 2023 California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged. Any commercial use must be negotiated with the Office
// of Technology Transfer at the California Institute of Technology.
// ======================================================================

#include "Svc/DpManager/test/ut/Rules/Testers.hpp"

namespace Svc {

  namespace Testers {

    BufferGetStatus::Tester bufferGetStatus;

    CLEAR_EVENT_THROTTLE::Tester clearEventThrottle;

    ProductGetIn::Tester productGetIn;

    ProductRequestIn::Tester productRequestIn;

    ProductSendIn::Tester productSendIn;

    SchedIn::Tester schedIn;

  }

}
