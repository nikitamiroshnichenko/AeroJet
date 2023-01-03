/*
 * ConstantPoolEntry.cpp
 *
 * Copyright © 2023 AeroJet Developers. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Java/ClassFile/ConstantPoolEntry.hpp"
#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Stream/Reader.hpp"
#include "Stream/StreamUtils.hpp"
#include "Stream/Writer.hpp"

namespace AeroJet::Java::ClassFile
{
    constexpr u2 CONSTANT_POOL_INFO_CLASS_DATA_SIZE = 2;
    constexpr u2 CONSTANT_POOL_INFO_FIELD_REF_SIZE = 4;
    constexpr u2 CONSTANT_POOL_INFO_STRING_SIZE = 2;
    constexpr u2 CONSTANT_POOL_INFO_INTEGER_SIZE = 4;
    constexpr u2 CONSTANT_POOL_INFO_LONG_SIZE = 8;
    constexpr u2 CONSTANT_POOL_INFO_NAME_AND_TYPE_SIZE = 4;
    constexpr u2 CONSTANT_POOL_INFO_METHOD_HANDLE_SIZE = 3;
    constexpr u2 CONSTANT_POOL_INFO_METHOD_TYPE_SIZE = 2;
    constexpr u2 CONSTANT_POOL_INFO_INVOKE_DYNAMIC_SIZE = 2;

    ConstantPoolEntry::ConstantPoolEntry(const ConstantPoolInfoTag tag, const std::vector<u1>& data) : m_tag(tag), m_data(data)
    {
    }

    ConstantPoolInfoTag ConstantPoolEntry::tag() const
    {
        return m_tag;
    }

    const std::vector<u1>& ConstantPoolEntry::data() const
    {
        return m_data;
    }

    ConstantPoolInfoUtf8::ConstantPoolInfoUtf8(const std::vector<u1>& bytes) : m_string(bytes.begin(), bytes.end())
    {
    }

    u2 ConstantPoolInfoUtf8::length()
    {
        return m_string.size();
    }

    std::string_view ConstantPoolInfoUtf8::asString() const
    {
        return m_string;
    }

    std::vector<u1> ConstantPoolInfoUtf8::bytes() const
    {
        return  std::vector<u1>{m_string.begin(), m_string.end()};
    }

    ConstantPoolInfoInteger::ConstantPoolInfoInteger(u4 bytes) : m_bytes(bytes)
    {
    }

    u4 ConstantPoolInfoInteger::bytes()
    {
        return m_bytes;
    }

    ConstantPoolInfoLong::ConstantPoolInfoLong(u4 highBytes, u4 lowBytes) : m_highBytes(highBytes), m_lowBytes(lowBytes)
    {
    }

    u4 ConstantPoolInfoLong::highBytes()
    {
        return m_highBytes;
    }

    u4 ConstantPoolInfoLong::lowBytes()
    {
        return m_lowBytes;
    }

    ConstantPoolInfoClass::ConstantPoolInfoClass(u2 nameIndex) : m_nameIndex(nameIndex)
    {
    }

    u2 ConstantPoolInfoClass::nameIndex()
    {
        return m_nameIndex;
    }

    ConstantPoolInfoString::ConstantPoolInfoString(u2 stringIndex) : m_stringIndex(stringIndex)
    {
    }

    u2 ConstantPoolInfoString::stringIndex()
    {
        return m_stringIndex;
    }

    ConstantPoolInfoFieldRef::ConstantPoolInfoFieldRef(u2 classIndex, u2 nameAndTypeIndex) : m_classIndex(classIndex), m_nameAndTypeIndex(nameAndTypeIndex)
    {
    }

    u2 ConstantPoolInfoFieldRef::classIndex()
    {
        return m_classIndex;
    }

    u2 ConstantPoolInfoFieldRef::nameAndTypeIndex()
    {
        return m_nameAndTypeIndex;
    }

    ConstantPoolInfoNameAndType::ConstantPoolInfoNameAndType(u2 nameIndex, u2 descriptorIndex) : m_nameIndex(nameIndex), m_descriptorIndex(descriptorIndex)
    {
    }

    u2 ConstantPoolInfoNameAndType::nameIndex()
    {
        return m_nameIndex;
    }

    u2 ConstantPoolInfoNameAndType::descriptorIndex()
    {
        return m_descriptorIndex;
    }

    ConstantPoolInfoMethodHandle::ConstantPoolInfoMethodHandle(ConstantPoolInfoMethodHandle::ReferenceKind referenceKind, u2 referenceIndex) :
        m_referenceKind(referenceKind),
        m_referenceIndex(referenceIndex)
    {
    }

    ConstantPoolInfoMethodHandle::ReferenceKind ConstantPoolInfoMethodHandle::referenceKind()
    {
        return m_referenceKind;
    }

    u2 ConstantPoolInfoMethodHandle::referenceIndex()
    {
        return m_referenceIndex;
    }

    ConstantPoolInfoMethodType::ConstantPoolInfoMethodType(u2 descriptorIndex) : m_descriptorIndex(descriptorIndex)
    {
    }

    u2 ConstantPoolInfoMethodType::descriptorIndex()
    {
        return m_descriptorIndex;
    }

    ConstantPoolInfoInvokeDynamic::ConstantPoolInfoInvokeDynamic(u2 bootstrapMethodAttributeIndex, u2 nameAndTypeIndex) :
        m_bootstrapMethodAttributeIndex(bootstrapMethodAttributeIndex),
        m_nameAndTypeIndex(nameAndTypeIndex)
    {
    }

    u2 ConstantPoolInfoInvokeDynamic::bootstrapMethodAttributeIndex()
    {
        return m_bootstrapMethodAttributeIndex;
    }

    u2 ConstantPoolInfoInvokeDynamic::nameAndTypeIndex()
    {
        return m_nameAndTypeIndex;
    }
}


template<>
AeroJet::Java::ClassFile::ConstantPoolEntry AeroJet::Stream::Reader::read(std::istream& stream)
{
    std::stringstream dataStream;

    const AeroJet::Java::ClassFile::ConstantPoolInfoTag tag = static_cast<AeroJet::Java::ClassFile::ConstantPoolInfoTag>(AeroJet::Stream::Reader::read<u1>(stream));
    switch (tag)
    {
        case Java::ClassFile::ConstantPoolInfoTag::UTF_8:
        {
            const AeroJet::u2 dataSize = AeroJet::Stream::Reader::read<AeroJet::u2>(stream);
            for (AeroJet::u2 byteIndex = 0; byteIndex < dataSize; byteIndex++)
            {
                AeroJet::u1 byte = AeroJet::Stream::Reader::read<AeroJet::u1>(stream);
                AeroJet::Stream::Writer::write(dataStream, byte);
            }

            break;
        }
        case Java::ClassFile::ConstantPoolInfoTag::INTEGER:
        case Java::ClassFile::ConstantPoolInfoTag::FLOAT:
        {
            const AeroJet::u4 value = AeroJet::Stream::Reader::read<AeroJet::u4>(stream);
            AeroJet::Stream::Writer::write(dataStream, value);

            break;
        }
        case Java::ClassFile::ConstantPoolInfoTag::LONG:
        case Java::ClassFile::ConstantPoolInfoTag::DOUBLE:
        {
            const AeroJet::u4 highBytes = AeroJet::Stream::Reader::read<AeroJet::u4>(stream);
            const AeroJet::u4 lowBytes = AeroJet::Stream::Reader::read<AeroJet::u4>(stream);

            AeroJet::Stream::Writer::write(dataStream, highBytes);
            AeroJet::Stream::Writer::write(dataStream, lowBytes);

            break;
        }
        case Java::ClassFile::ConstantPoolInfoTag::CLASS:
        case Java::ClassFile::ConstantPoolInfoTag::STRING:
        case Java::ClassFile::ConstantPoolInfoTag::METHOD_TYPE:
        {
            const AeroJet::u2 index = AeroJet::Stream::Reader::read<AeroJet::u2>(stream);
            AeroJet::Stream::Writer::write(dataStream, index);

            break;
        }
        case Java::ClassFile::ConstantPoolInfoTag::FIELD_REF:
        case Java::ClassFile::ConstantPoolInfoTag::METHOD_REF:
        case Java::ClassFile::ConstantPoolInfoTag::INTERFACE_METHOD_REF:
        case Java::ClassFile::ConstantPoolInfoTag::NAME_AND_TYPE:
        case Java::ClassFile::ConstantPoolInfoTag::INVOKE_DYNAMIC:
        {
            const AeroJet::u2 index1 = AeroJet::Stream::Reader::read<AeroJet::u2>(stream);
            const AeroJet::u2 index2 = AeroJet::Stream::Reader::read<AeroJet::u2>(stream);

            AeroJet::Stream::Writer::write(dataStream, index1);
            AeroJet::Stream::Writer::write(dataStream, index2);

            break;
        }
        case Java::ClassFile::ConstantPoolInfoTag::METHOD_HANDLE:
        {
            const AeroJet::u1 referenceKind = AeroJet::Stream::Reader::read<AeroJet::u1>(stream);
            const AeroJet::u2 referenceIndex = AeroJet::Stream::Reader::read<AeroJet::u2>(stream);

            AeroJet::Stream::Writer::write(dataStream, referenceKind);
            AeroJet::Stream::Writer::write(dataStream, referenceIndex);

            break;
        }
        default:
            throw AeroJet::Exceptions::RuntimeException(
                    fmt::format(
                            "Unexpected Constant Pool Entry Tag {:#04x} at position {:#08x}!",
                            static_cast<u2>(tag),
                            stream.tellg()
                    ));
    }

    std::vector<AeroJet::u1> dataBytes = AeroJet::Stream::Utils::streamToBytes(dataStream);
    return {tag, dataBytes};
}
