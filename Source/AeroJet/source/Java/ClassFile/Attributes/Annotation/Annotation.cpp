/*
 * Annotation.cpp
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

#include "Java/ClassFile/Attributes/Annotation/Annotation.hpp"

namespace AeroJet::Java::ClassFile
{
    Annotation::Annotation(u2 typeIndex, const std::vector<ElementValuePair>& elementValuePairs) :
        m_typeIndex(typeIndex), m_elementValuePairs(elementValuePairs)
    {
    }

    u2 Annotation::typeIndex() const
    {
        return m_typeIndex;
    }

    u2 Annotation::numElementValuePairs() const
    {
        return m_elementValuePairs.size();
    }

    const std::vector<ElementValuePair>& Annotation::elementValuePairs() const
    {
        return m_elementValuePairs;
    }
} // namespace AeroJet::Java::ClassFile
