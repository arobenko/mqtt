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

using VarByteInt =
    comms::field::IntValue<
        comms::Field<comms::option::LittleEndian>,
        std::uint32_t,
        comms::option::VarLength<1, 4>
    >;

template <typename... TOpts>
using String =
    comms::field::String<
        FieldBase,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<
                FieldBase,
                std::uint16_t
            >
        >,
        TOpts...
    >;

template <typename... TOpts>
using BinData =
    comms::field::ArrayList<
        FieldBase,
        std::uint8_t,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<
                FieldBase,
                std::uint16_t
            >
        >,
        TOpts...
    >;


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

using RemSize = VarByteInt;

template <typename... TOpts>
struct ProtocolName : public
    comms::field::String<
        FieldBase,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<FieldBase, std::uint16_t>
        >,
        TOpts...
    >
{
    ProtocolName()
    {
        comms::field::toFieldBase(*this).value() = "MQTT";
    }

    bool valid() const
    {
        auto& asBase = comms::field::toFieldBase(*this);
        return asBase.valid() && (asBase.value() == "MQTT");
    }
};

template <ProtocolVersionVal TVer>
using ProtocolVersion =
    comms::field::EnumValue<
        FieldBase,
        ProtocolVersionVal,
        comms::option::DefaultNumValue<(int)TVer>,
        comms::option::ValidNumValueRange<(int)TVer, (int)TVer>
    >;

struct ConnectFlagsLow : public
    comms::field::BitmaskValue<
        FieldBase,
        comms::option::FixedBitLength<3>,
        comms::option::BitmaskReservedBits<0x1, 0>
    >
{
    COMMS_BITMASK_BITS(cleanSession=1, willFlag);
};

struct ConnectFlagsHigh : public
    comms::field::BitmaskValue<
        FieldBase,
        comms::option::FixedBitLength<3>,
        comms::option::BitmaskReservedBits<0x1, 0>
    >
{
    COMMS_BITMASK_BITS(willRetain, password, username);
};

enum class QosVal : std::uint8_t
{
    AtMostOnceDelivery,
    AtLeastOnceDelivery,
    ExactlyOnceDelivery,
    NumOfValues
};

template <typename... TExtraOptions>
using QoS = comms::field::EnumValue<
        FieldBase,
        QosVal,
        comms::option::ValidNumValueRange<0, (int)QosVal::NumOfValues - 1>,
        TExtraOptions...
    >;

class ConnectFlags : public
    comms::field::Bitfield<
        FieldBase,
        std::tuple<
            field::ConnectFlagsLow,
            field::QoS<comms::option::FixedBitLength<2> >,
            field::ConnectFlagsHigh
        >
    >
{
public:
    COMMS_FIELD_MEMBERS_ACCESS(flagsLow, willQos, flagsHigh);

    bool valid() const
    {
        typedef typename std::decay<decltype(field_flagsLow())>::type LowFlagsFieldType;
        typedef typename std::decay<decltype(field_willQos())>::type QosFieldType;
        typedef typename std::decay<decltype(field_flagsHigh())>::type HighFlagsFieldType;

        if (!field_flagsLow().getBitValue(LowFlagsFieldType::BitIdx_willFlag)) {
            if (field_willQos().value() != QosFieldType::ValueType::AtMostOnceDelivery) {
                return false;
            }

            if (field_flagsHigh().getBitValue(HighFlagsFieldType::BitIdx_willRetain)) {
                return false;
            }
        }

        if ((!field_flagsHigh().getBitValue(HighFlagsFieldType::BitIdx_username)) &&
            (field_flagsHigh().getBitValue(HighFlagsFieldType::BitIdx_password))) {
            return false;
        }

        return true;
    }
};

using KeepAlive = comms::field::IntValue<FieldBase, std::uint16_t, comms::option::UnitsSeconds>;

template <typename... TOpts>
using ClientId = String<TOpts...>;

template <typename... TOpts>
using WillTopic = comms::field::Optional<String<TOpts...> >;

template <typename... TOpts>
using WillMessage = comms::field::Optional<BinData<TOpts...> >;

template <typename... TOpts>
using UserName = comms::field::Optional<String<TOpts...> >;

template <typename... TOpts>
using Password = comms::field::Optional<BinData<TOpts...> >;

} // namespace field

} // namespace common

}  // namespace protocol

}  // namespace mqtt


