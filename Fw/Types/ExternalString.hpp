// ======================================================================
// @file   ExternalString.hpp
// @author Robert Bocchino
// @brief  A string backed by an external buffer
// ======================================================================

#ifndef FW_EXTERNAL_STRING_HPP
#define FW_EXTERNAL_STRING_HPP

#include <FpConfig.hpp>

#include "Fw/Types/StringType.hpp"
#include "Fw/Types/StringUtils.hpp"

namespace Fw {

//! A string backed by an external buffer
class ExternalString : public Fw::StringBase {
  public:
    // ----------------------------------------------------------------------
    // Construction and destruction
    // ----------------------------------------------------------------------

    //! Constructor (bufferPtr and bufferSize)
    ExternalString(char* bufferPtr,                 //!< The buffer pointer
                   StringBase::SizeType bufferSize  //!< The buffer size
                   )
        : m_bufferPtr(bufferPtr), m_bufferSize(bufferSize) {}

    //! Constructor (bufferPtr, bufferSize, and StringBase)
    ExternalString(char* bufferPtr,                  //!< The buffer pointer
                   StringBase::SizeType bufferSize,  //!< The buffer size
                   const StringBase& sb              //!< The source string
                   )
        : m_bufferPtr(bufferPtr), m_bufferSize(bufferSize) {
        *this = sb;
    }

    //! Constructor (bufferPtr, bufferSize, and const char*)
    ExternalString(char* bufferPtr,                  //!< The buffer pointer
                   StringBase::SizeType bufferSize,  //!< The buffer size
                   const char* str                   //!< The source string
                   )
        : m_bufferPtr(bufferPtr), m_bufferSize(bufferSize) {
        *this = str;
    }

    //! Destructor
    ~ExternalString() {}

  public:
    // ----------------------------------------------------------------------
    // StringBase interface
    // ----------------------------------------------------------------------

    //! Gets the char buffer
    //! @return The char buffer
    const char* toChar() const { return this->m_bufferPtr; }

    //! Returns the buffer size
    //! @return The buffer size
    StringBase::SizeType getCapacity() const { return this->m_bufferSize; }

  public:
    // ----------------------------------------------------------------------
    // Operators
    // ----------------------------------------------------------------------

    // Operator= (const ExternalString&)
    ExternalString& operator=(const ExternalString& src) {
        (void)StringBase::operator=(src);
        return *this;
    }

    // Operator= (const StringBase&)
    ExternalString& operator=(const StringBase& src) {
        (void)StringBase::operator=(src);
        return *this;
    }

    // const char* assignment operator
    ExternalString& operator=(const char* src) {
        (void)StringBase::operator=(src);
        return *this;
    }

  private:
    // ----------------------------------------------------------------------
    // Data members
    // ----------------------------------------------------------------------

    //! Pointer to string buffer
    char* m_bufferPtr;

    //! Size of string buffer
    //! F Prime strings are null-terminated, so this is one more than
    //! the length of the largest string that the buffer can hold
    StringBase::SizeType m_bufferSize;
};
}  // namespace Fw

#endif
