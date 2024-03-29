/*
 * RuntimeVisibleParameterAnnotations.hpp
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

#pragma once

#include "Java/ClassFile/Attributes/Annotation/ParameterAnnotation.hpp"
#include "Java/ClassFile/Attributes/Attribute.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    /**
     * The RuntimeVisibleParameterAnnotations attribute is a variable-length attribute in the attributes table of
     * the method_info structure (§4.6). The RuntimeVisibleParameterAnnotations attribute records run-time visible
     * annotations on the declarations of formal parameters of the corresponding method. The Java Virtual Machine must
     * make these annotations available so they can be returned by the appropriate reflective APIs.
     *
     * There may be at most one RuntimeVisibleParameterAnnotations attribute in the attributes table
     * of a method_info structure.
     */
    class RuntimeVisibleParameterAnnotations : public Attribute
    {
      public:
        static constexpr auto RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS_ATTRIBUTE_NAME =
            "RuntimeVisibleParameterAnnotations";

        RuntimeVisibleParameterAnnotations(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        /**
         * The value of the num_parameters item gives the number of formal parameters of the method represented by the
         * method_info structure on which the annotation occurs.
         */
        [[nodiscard]] u1 numParameters() const;

        /**
         * Each entry in the parameter_annotations table represents all of the run-time visible annotations on
         * the declaration of a single formal parameter. The i'th entry in the table corresponds to the i'th formal
         * parameter in the method descriptor (§4.3.3).
         */
        [[nodiscard]] const std::vector<ParameterAnnotation>& parameterAnnotations() const;

      private:
        std::vector<ParameterAnnotation> m_parameterAnnotations;
    };
} // namespace AeroJet::Java::ClassFile