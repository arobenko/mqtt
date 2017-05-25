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

#include <cstdint>
#include <tuple>
#include <vector>

#include "comms/Message.h"
#include "comms/fields.h"

#include "MsgId.h"

namespace mqtt
{

namespace protocol
{

namespace common
{

template <typename... TOptions>
class Message : public
    comms::Message<
        comms::option::BigEndian,
        comms::option::MsgIdType<MsgId>,
        TOptions...
    >
{
public:
    using FlagsField = comms::field::IntValue<
        common::field::FieldBase,
        std::uint8_t,
        comms::option::ValidNumValueRange<0x0, 0xf>,
        comms::option::FailOnInvalid<comms::ErrorStatus::ProtocolError>
    >;

    Message() = default;
    Message(const Message&) = default;
    Message(Message&&) = default;
    ~Message() = default;

    Message& operator=(const Message&) = default;
    Message& operator=(Message&&) = default;

    const FlagsField& getFlags() const
    {
        return m_flags;
    }

    void setFlags(const FlagsField& flags)
    {
        m_flags = flags;
    }


private:
    FlagsField m_flags;
};

} // namespace common

}  // namespace protocol

}  // namespace mqtt
