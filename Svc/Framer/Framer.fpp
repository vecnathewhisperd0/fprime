module Svc {

  @ A component for deframing input for transmission to the ground
  passive component Framer {

    # ----------------------------------------------------------------------
    # Receiving packets
    # ----------------------------------------------------------------------

    @ Port for receiving data packets stored in statically-sized
    @ Com buffers
    guarded input port comIn: Fw.Com

    @ Port for receiving data packets stored in dynamically-sized
    @ managed bufers
    guarded input port bufferIn: Fw.BufferSend

    # ----------------------------------------------------------------------
    # Allocation and deallocation of buffers 
    # ----------------------------------------------------------------------

    @ Port for deallocating buffers received on bufferIn, after
    @ copying packet data to the frame buffer
    output port bufferDeallocate: Fw.BufferSend

    @ Port for allocating buffers to hold framed data
    output port framedAllocate: Fw.BufferGet

    # ----------------------------------------------------------------------
    # Sending frame data 
    # ----------------------------------------------------------------------

    @ Port for sending buffers containing framed data. Ownership of the
    @ buffer passes to the receiver.
    output port framedOut: Drv.ByteStreamSend

  }

}
