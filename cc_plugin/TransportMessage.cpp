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

#include "TransportMessage.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

void TransportMessage::updateFieldPropertiesImpl(QWidget& fieldWidget, uint idx) const
{
    static const char* FieldNames[] = {
        "ID",
        "Size",
        "Data"
    };

    static const std::size_t NumOfFields =
        std::tuple_size<cc_plugin::Stack::AllFields>::value;

    static_assert(
        std::extent<decltype(FieldNames)>::value == NumOfFields,
        "FieldNames array must be updated.");

    if (NumOfFields <= idx) {
        return;
    }

    cc::Property::setNameVal(fieldWidget, FieldNames[idx]);
}

}  // namespace cc_plugin

}  // namespace mqtt

