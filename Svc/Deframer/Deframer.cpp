// ======================================================================
// \title  Deframer.cpp
// \author mstarch, bocchino
// \brief  cpp file for Deframer component implementation class
//
// \copyright
// Copyright 2009-2022, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include "Fw/Com/ComPacket.hpp"
#include "Fw/Logger/Logger.hpp"
#include "Fw/Types/BasicTypes.hpp"
#include "Svc/Deframer/Deframer.hpp"

namespace Svc {

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Deframer ::Deframer(const char* const compName) : 
    DeframerComponentBase(compName), 
    DeframingProtocolInterface(),
    m_protocol(nullptr),
    m_in_ring(m_ring_buffer, sizeof(m_ring_buffer))
{

}

void Deframer ::init(const NATIVE_INT_TYPE instance) {
    DeframerComponentBase::init(instance);
}

Deframer ::~Deframer() {}

void Deframer ::setup(DeframingProtocol& protocol) {
    // Check that this is the first time we are calling setup
    FW_ASSERT(m_protocol == nullptr);
    // Assign the protocol passed in to m_protocol
    m_protocol = &protocol;
    // Pass *this as the DeframingProtocolInstance to protocol setup
    // Deframer is derived from and implements DeframingProtocolInterface
    protocol.setup(*this);
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Deframer ::cmdResponseIn_handler(
    NATIVE_INT_TYPE portNum,
    FwOpcodeType opcode,
    U32 cmdSeq,
    const Fw::CmdResponse& response
) {
    // Nothing to do
}

void Deframer ::framedIn_handler(
    const NATIVE_INT_TYPE portNum,
    Fw::Buffer& recvBuffer,
    const Drv::RecvStatus& recvStatus
) {
    // Check whether there is data to process
    if (Drv::RecvStatus::RECV_OK == recvStatus.e) {
        // There is: process the data
        processBuffer(recvBuffer);
    }
    // Deallocate the buffer
    framedDeallocate_out(0, recvBuffer);
}

void Deframer ::schedIn_handler(
    const NATIVE_INT_TYPE portNum,
    NATIVE_UINT_TYPE context
) {
    // Check for data
    Fw::Buffer buffer(m_poll_buffer, sizeof(m_poll_buffer));
    auto status = framedPoll_out(0, buffer);
    if (status == Drv::PollStatus::POLL_OK) {
        // Data exists: process it
        processBuffer(buffer);
    }
}

// ---------------------------------------------------------------------- 
// Implementation of DeframingProtocolInterface
// ----------------------------------------------------------------------

Fw::Buffer Deframer ::allocate(const U32 size)  {
    return bufferAllocate_out(0, size);
}

void Deframer ::route(Fw::Buffer& packetBuffer) {

    // Read the packet type from the packet buffer
    FwPacketDescriptorType packetType = Fw::ComPacket::FW_PACKET_UNKNOWN;
    auto status = Fw::FW_SERIALIZE_OK;
    {
        auto& serial = packetBuffer.getSerializeRepr();
        serial.setBuffLen(packetBuffer.getSize());
        status = serial.deserialize(packetType);
    }

    // Whethter to deallocate the packet buffer
    bool deallocate = true;

    // Process the packet
    if (status == Fw::FW_SERIALIZE_OK) {
        U8 *const packetData = packetBuffer.getData();
        const auto packetSize = packetBuffer.getSize();
        switch (packetType) {
            case Fw::ComPacket::FW_PACKET_COMMAND: {
                // Allocate a com buffer on the stack
                Fw::ComBuffer com;
                // Copy the contents of the packet buffer into the com buffer
                com.setBuff(packetData, packetSize);
                // Send the com buffer
                comOut_out(0, com, 0);
                break;
            }
            case Fw::ComPacket::FW_PACKET_FILE: {
                // If file uplink is possible, handle the file packet.
                // Otherwise ignore it.
                if (isConnected_bufferOut_OutputPort(0)) {
                    // Shift the packet buffer to skip the packet type
                    // The FileUplink component does not expect the packet
                    // type to be there.
                    packetBuffer.setData(packetData + sizeof(packetType));
                    packetBuffer.setSize(packetSize - sizeof(packetType));
                    // Send the packet buffer
                    bufferOut_out(0, packetBuffer);
                    // Transfer ownership of the buffer to the receiver
                    deallocate = false;
                }
                break;
            }
            default:
                break;
        }
    }

    if (deallocate) {
        // Deallocate the packet buffer
        bufferDeallocate_out(0, packetBuffer);
    }

}

// ----------------------------------------------------------------------
// Helper methods
// ----------------------------------------------------------------------

void Deframer ::processBuffer(Fw::Buffer& buffer) {

    const auto bufferSize = buffer.getSize();
    U8 *const bufferData = buffer.getData();
    // Current offset into buffer
    U32 offset = 0;
    // Remaining data in buffer
    U32 remaining = bufferSize;

    for (U32 i = 0; i < bufferSize; ++i) {
        // If there is no data left, exit the loop
        if (remaining == 0) {
            break;
        }
        // Compute the size of data to serialize
        const NATIVE_UINT_TYPE ringFreeSize = m_in_ring.get_free_size();
        const NATIVE_UINT_TYPE serSize = (ringFreeSize <= remaining) ?
            ringFreeSize : static_cast<NATIVE_UINT_TYPE>(remaining);
        // Serialize data into the ring buffer
        const auto status = m_in_ring.serialize(&bufferData[offset], serSize);
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK, status);
        // Process the data
        processRing();
        // Update buffer offset and remaining
        offset += serSize;
        remaining -= serSize;
    }

    // In every iteration, either remaining == 0 and we break out
    // of the loop, or we consume at least one byte from the buffer.
    // So there should be no data left in the buffer.
    FW_ASSERT(remaining == 0, remaining);

}

void Deframer ::processRing() {

    FW_ASSERT(m_protocol != nullptr);

    // The number of remaining bytes in the ring buffer
    U32 remaining = 0;
    // The protocol status
    auto status = DeframingProtocol::DEFRAMING_STATUS_SUCCESS;
    // The ring buffer capacity
    const auto ringCapacity = m_in_ring.get_capacity();

    // Process the ring buffer looking for at least the header
    for (U32 i = 0; i < ringCapacity; i++) {
        // Get the number of bytes remaining in the ring buffer
        remaining = m_in_ring.get_allocated_size();
        // If there are none, we are done
        if (remaining == 0) {
            break;
        }
        // Needed is an out-only variable
        // Initialize it to zero
        U32 needed = 0;
        // Call the deframe method of the protocol, getting
        // needed and status
        status = m_protocol->deframe(m_in_ring, needed);
        // Deframing protocol must not consume data in the ring buffer
        FW_ASSERT(
            m_in_ring.get_allocated_size() == remaining,
            m_in_ring.get_allocated_size(),
            remaining
        );
        // On successful deframing, consume data from the ring buffer now
        if (status == DeframingProtocol::DEFRAMING_STATUS_SUCCESS) {
            // If deframing succeeded, protocol should set needed
            // to a non-zero value
            FW_ASSERT(needed != 0);
            FW_ASSERT(needed <= remaining, needed, remaining);
            m_in_ring.rotate(needed);
            FW_ASSERT(
                m_in_ring.get_allocated_size() == remaining - needed,
                m_in_ring.get_allocated_size(),
                remaining,
                needed
            );
        }
        // More data needed
        else if (status == DeframingProtocol::DEFRAMING_MORE_NEEDED) {
            // Deframing protocol should not report "more is needed" 
            // unless more is needed
            FW_ASSERT(needed > remaining, needed, remaining);
            // Break out of loop: suspend deframing until we receive
            // another buffer
            break;
        }
        // Error occurred
        else {
            // Skip one byte of bad data
            m_in_ring.rotate(1);
            FW_ASSERT(
                m_in_ring.get_allocated_size() == remaining - 1,
                m_in_ring.get_allocated_size(),
                remaining
            );
            // Log checksum errors
            if (status == DeframingProtocol::DEFRAMING_INVALID_CHECKSUM) {
                Fw::Logger::logMsg("[ERROR] Deframing checksum validation failed\n");
            }
        }
    }

    // If more not needed, circular buffer should be empty
    if (status != DeframingProtocol::DEFRAMING_MORE_NEEDED) {
        FW_ASSERT(remaining == 0, remaining);
    }

}

}  // end namespace Svc
