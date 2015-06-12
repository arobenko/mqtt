//
// Copyright 2015 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <type_traits>
#include <functional>
#include <cassert>

#include "Pubrec.h"

#include "cc_plugin/field/PacketId.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace message
{

const char* Pubrec::nameImpl() const
{
    static const char* Str = "PUBREC";
    return Str;
}

void Pubrec::updateFieldPropertiesImpl(QWidget& fieldWidget, uint idx) const
{
    typedef std::function<void (QObject&)> FieldUpdateFunc;
    static const FieldUpdateFunc FuncMap[] = {
        &cc_plugin::field::updatePacketIdProperties,
    };

    static const unsigned FuncsCount = std::extent<decltype(FuncMap)>::value;

    static_assert(FuncsCount == FieldIdx_NumOfValues,
        "The funcs map is incorrect");

    if (FuncsCount <= idx) {
        return;
    }

    assert(FuncMap[idx]);
    FuncMap[idx](fieldWidget);
}


}  // namespace message

}  // namespace cc_plugin

}  // namespace mqtt

