//
// Copyright 2015 - 2016 (C). Alex Robenko. All rights reserved.
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

#include <tuple>
#include "mqtt/protocol/common/message/Connack.h"
#include "mqtt/protocol/v311/field.h"

namespace mqtt
{

namespace protocol
{

namespace v311
{

namespace message
{

using ConnackFields =
    std::tuple<
        common::field::ConnackFlags,
        v311::field::ConnackResponseCode
    >;


template <typename TMsgBase>
class Connack : public
        common::message::Connack<
            TMsgBase,
            ConnackFields,
            Connack<TMsgBase>
        >
{
public:
    COMMS_MSG_FIELDS_ACCESS(flags, responseCode);

    Connack() = default;
    Connack(const Connack&) = default;
    Connack(Connack&& other) = default;
    ~Connack() = default;

    Connack& operator=(const Connack&) = default;
    Connack& operator=(Connack&&) = default;
};

} // namespace message

} // namespace v311

} // namespace protocol

} // namespace mqtt
