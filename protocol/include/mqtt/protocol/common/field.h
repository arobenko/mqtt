//
// Copyright 2015 - 2017 (C). Alex Robenko. All rights reserved.
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

#include "comms/fields.h"

#include "MsgId.h"

namespace mqtt
{

namespace protocol
{

namespace common
{

namespace field
{

using FieldBase = comms::Field<comms::option::BigEndian>;

enum class ProtocolVersionVal : std::uint8_t
{
    v311 = 4,
    v5 = 5
};

namespace details
{

constexpr MsgId maxMsgId(ProtocolVersionVal ver)
{
    return ver <= ProtocolVersionVal::v311 ? common::MsgId_DISCONNECT : common::MsgId_AUTH;
}

} // namespace details


template <ProtocolVersionVal TVer = ProtocolVersionVal::v311>
using MsgIdField =
    comms::field::EnumValue<
        FieldBase,
        MsgId,
        comms::option::FixedBitLength<4>,
        comms::option::DefaultNumValue<MsgId_CONNECT>,
        comms::option::FailOnInvalid<comms::ErrorStatus::ProtocolError>,
        comms::option::ValidNumValueRange<common::MsgId_CONNECT, details::maxMsgId(TVer)>
    >;

using Flags =
    comms::field::IntValue<
        FieldBase,
        std::uint8_t,
        comms::option::FixedBitLength<4>
    >;

template <ProtocolVersionVal TVer = ProtocolVersionVal::v311>
struct MsgIdFlagsBundle : public
        comms::field::Bitfield<
            FieldBase,
            std::tuple<
                Flags,
                MsgIdField<TVer>
            >
        >
{
    COMMS_FIELD_MEMBERS_ACCESS(flags, id);
};

using RemSize =
    comms::field::IntValue<
        comms::Field<comms::option::LittleEndian>,
        std::uint32_t,
        comms::option::VarLength<1, 4>
    >;

} // namespace field

} // namespace common

}  // namespace protocol

}  // namespace mqtt


