// ======================================================================
// \title  SerializableFile.hpp
// \author dinkel
// \brief  hpp file for SerializableFile
//
// \copyright
// Copyright 2009-2016, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef Fw_SerializableFile_HPP
#define Fw_SerializableFile_HPP

#include <Fw/Types/Serializable.hpp>
#include <Fw/Types/MemAllocator.hpp>
#include <Fw/Types/SerialBuffer.hpp>

namespace Fw {

  //! The type of a packet header
  class SerializableFile {

    public:
      enum Status {
        OP_OK,
        FILE_OPEN_ERROR,
        FILE_WRITE_ERROR,
        FILE_READ_ERROR,
        DESERIALIZATION_ERROR
      };

      // NOTE!: This should not be used with an allocator that can return a smaller buffer than requested
      SerializableFile(MemAllocator* allocator, NATIVE_UINT_TYPE maxSerializedSize);
      ~SerializableFile();

      Status load(const char* fileName, Serializable& serializable);
      Status save(const char* fileName, Serializable& serializable);

    private:
      void reset();
      MemAllocator* allocator;
      bool recoverable; // don't care; for allocator
      NATIVE_UINT_TYPE actualSize; // for checking
      SerialBuffer buffer;
  };
}

#endif
