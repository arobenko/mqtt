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


#pragma once

#include <cstdint>

namespace mqtt
{

namespace field
{

enum class QosType : std::uint8_t
{
    AtMostOnceDelivery,
    AtLeastOnceDelivery,
    ExactlyOnceDelivery,
    NumOfValues
};

template <typename TFieldBase, typename... TExtraOptions>
using QoS = comms::field::EnumValue<
        TFieldBase,
        QosType,
        comms::option::ValidNumValueRange<(int)QosType::AtMostOnceDelivery, (int)QosType::NumOfValues - 1>,
        comms::option::FailOnInvalid,
        TExtraOptions...
    >;

}  // namespace field

}  // namespace mqtt



