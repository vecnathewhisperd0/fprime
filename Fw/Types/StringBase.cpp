/**
 * \file StringBase.cpp
 * \author T. Canham
 * \brief Implements F Prime string base class
 *
 * \copyright
 * Copyright 2009-2016, by the California Institute of Technology.
 * ALL RIGHTS RESERVED.  United States Government Sponsorship
 * acknowledged.
 *
 */

#include <Fw/Types/Assert.hpp>
#include <Fw/Types/StringType.hpp>
#include <Fw/Types/StringUtils.hpp>
#include <cstdarg>
#include <cstdio>
#include <cstring>

namespace Fw {

StringBase::StringBase() {}

StringBase::~StringBase() {}

const CHAR* StringBase::operator+=(const CHAR* src) {
    this->appendBuff(src, StringUtils::string_length(src, this->getCapacity()));
    return this->toChar();
}

const StringBase& StringBase::operator+=(const StringBase& src) {
    this->appendBuff(src.toChar(), src.length());
    return *this;
}

bool StringBase::operator==(const StringBase& other) const {
    SizeType len = this->length();
    if (len != other.length()) {
        return false;
    } else {
        return this->operator==(other.toChar());
    }
}

bool StringBase::operator==(const CHAR* other) const {
    const CHAR* const us = this->toChar();
    if ((us == nullptr) or (other == nullptr)) {
        return false;
    }

    const SizeType capacity = this->getCapacity();
    const size_t result = strncmp(us, other, capacity);
    return (result == 0);
}

void StringBase::format(const CHAR* formatString, ...) {
    CHAR* us = const_cast<CHAR*>(this->toChar());
    SizeType cap = this->getCapacity();
    FW_ASSERT(us);
    va_list args;
    va_start(args, formatString);
    (void)vsnprintf(us, cap, formatString, args);
    va_end(args);
    // null terminate
    us[cap - 1] = 0;
}

bool StringBase::operator!=(const StringBase& other) const {
    return !operator==(other);
}

bool StringBase::operator!=(const CHAR* other) const {
    return !operator==(other);
}

#if FW_SERIALIZABLE_TO_STRING || BUILD_UT
void StringBase::toString(StringBase& text) const {
    text = *this;
}
#endif

#ifdef BUILD_UT
std::ostream& operator<<(std::ostream& os, const StringBase& str) {
    os << str.toChar();
    return os;
}
#endif

StringBase& StringBase::operator=(const StringBase& other) {
    if (this != &other) {
        (void)Fw::StringUtils::string_copy(const_cast<char*>(this->toChar()), other.toChar(), this->getCapacity());
    }
    return *this;
}

// Copy constructor doesn't make sense in this virtual class as there is nothing to copy. Derived classes should
// call the empty constructor and then call their own copy function
StringBase& StringBase::operator=(const CHAR* other) {  // lgtm[cpp/rule-of-two]
    (void)Fw::StringUtils::string_copy(const_cast<char*>(this->toChar()), other, this->getCapacity());
    return *this;
}

void StringBase::appendBuff(const CHAR* buff, SizeType size) {
    const SizeType capacity = this->getCapacity();
    const SizeType length = this->length();
    FW_ASSERT(capacity > length, capacity, length);
    // Subtract 1 to leave space for null terminator
    SizeType remaining = capacity - length - 1;
    if (size < remaining) {
        remaining = size;
    }
    FW_ASSERT(remaining < capacity, remaining, capacity);
    (void)strncat(const_cast<CHAR*>(this->toChar()), buff, remaining);
}

StringBase::SizeType StringBase::length() const {
    const SizeType length = static_cast<SizeType>(StringUtils::string_length(this->toChar(), this->getCapacity()));
    FW_ASSERT(length <= this->maxLength(), static_cast<FwAssertArgType>(length),
              static_cast<FwAssertArgType>(this->maxLength()));
    return length;
}

StringBase::SizeType StringBase::maxLength() const {
    const SizeType capacity = this->getCapacity();
    FW_ASSERT(capacity > 0, static_cast<FwAssertArgType>(capacity));
    return capacity - 1;
}

StringBase::SizeType StringBase::serializedSize() const {
    return static_cast<SizeType>(sizeof(FwSizeStoreType)) + this->length();
}

StringBase::SizeType StringBase::serializedTruncatedSize(FwSizeType maxLength) const {
    return static_cast<SizeType>(sizeof(FwSizeStoreType)) + FW_MIN(this->length(), maxLength);
}

SerializeStatus StringBase::serialize(SerializeBufferBase& buffer) const {
    return buffer.serialize(reinterpret_cast<const U8*>(this->toChar()), this->length());
}

SerializeStatus StringBase::serialize(SerializeBufferBase& buffer, SizeType maxLength) const {
    const FwSizeType len = FW_MIN(maxLength, this->length());
    // serialize length and then bytes
    return buffer.serialize(reinterpret_cast<const U8*>(this->toChar()), len, Serialization::INCLUDE_LENGTH);
}

SerializeStatus StringBase::deserialize(SerializeBufferBase& buffer) {
    // Get the max size of the deserialized string
    const FwSizeType maxSize = this->maxLength();
    // Initial estimate of actual size is max size
    // This estimate is refined when calling the deserialize function below
    SizeType actualSize = maxSize;
    // Public interface returns const char*, but implementation needs char*
    // So use const_cast
    CHAR* raw = const_cast<CHAR*>(this->toChar());
    // Deserialize length
    // Fail if length exceeds max size (the initial value of actualSize)
    // Otherwise deserialize length bytes and set actualSize to length
    SerializeStatus stat = buffer.deserialize(reinterpret_cast<U8*>(raw), actualSize, Serialization::INCLUDE_LENGTH);
    if (stat == FW_SERIALIZE_OK) {
        // Deserialization succeeded: null-terminate string at actual size
        FW_ASSERT(actualSize <= maxSize, static_cast<FwAssertArgType>(actualSize),
                  static_cast<FwAssertArgType>(maxSize));
        raw[actualSize] = 0;
    } else {
        // Deserialization failed: leave string unmodified, but ensure that it
        // is null-terminated
        raw[0] = maxSize;
    }
    return stat;
}
}  // namespace Fw
