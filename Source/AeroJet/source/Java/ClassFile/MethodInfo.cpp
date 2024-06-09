/*
 * MethodInfo.cpp
 *
 * Copyright © 2024 AeroJet Developers. All Rights Reserved.
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

#include "Java/ClassFile/MethodInfo.hpp"

#include "Stream/StandardStreamWrapper.hpp"

namespace AeroJet::Java::ClassFile
{
    MethodInfo::MethodInfo(u2 accessFlags,
                           u2 nameIndex,
                           u2 descriptorIndex,
                           const std::vector<AttributeInfo>& attributes) :
        m_accessFlags(accessFlags),
        m_nameIndex(nameIndex), m_descriptorIndex(descriptorIndex), m_attributes(attributes)
    {
    }

    MethodInfo::AccessFlags MethodInfo::accessFlags() const
    {
        return static_cast<AccessFlags>(m_accessFlags);
    }

    u2 MethodInfo::nameIndex() const
    {
        return m_nameIndex;
    }

    u2 MethodInfo::descriptorIndex() const
    {
        return m_descriptorIndex;
    }

    const std::vector<AttributeInfo>& MethodInfo::attributes() const
    {
        return m_attributes;
    }
} // namespace AeroJet::Java::ClassFile

template<>
AeroJet::Java::ClassFile::MethodInfo AeroJet::Stream::Reader::read(std::istream& stream, ByteOrder byteOrder)
{
    const AeroJet::u2 accessFlags = read<AeroJet::u2>(stream, byteOrder);
    const AeroJet::u2 nameIndex = read<AeroJet::u2>(stream, byteOrder);
    const AeroJet::u2 descriptorIndex = read<AeroJet::u2>(stream, byteOrder);
    const AeroJet::u2 attributesCount = read<AeroJet::u2>(stream, byteOrder);

    std::vector<AeroJet::Java::ClassFile::AttributeInfo> attributes;
    attributes.reserve(attributesCount);
    for(int32_t attributeIndex = 0; attributeIndex < attributesCount; attributeIndex++)
    {
        attributes.emplace_back(
            AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::AttributeInfo>(stream, byteOrder));
    }

    return { accessFlags, nameIndex, descriptorIndex, attributes };
}
