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

template <typename... TOpt>
using ReservedBits =
    comms::field::IntValue<
        FieldBase,
        std::uint8_t,
        comms::option::ValidNumValueRange<0, 0>,
        TOpt...
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

struct ProtocolName : public String<>
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

using ClientId = String<>;

using WillTopic = comms::field::Optional<String<> >;

using WillMessage = comms::field::Optional<BinData<> >;

using UserName = comms::field::Optional<String<> >;

using Password = comms::field::Optional<BinData<> >;

struct ConnackFlags : public
    comms::field::BitmaskValue<
        FieldBase,
        comms::option::FixedLength<1>,
        comms::option::BitmaskReservedBits<0xfe, 0x0>
    >
{
    COMMS_BITMASK_BITS(sessionPresent);
};

class SingleBitBitmask :
    public comms::field::BitmaskValue<
        FieldBase,
        comms::option::FixedBitLength<1>
    >
{
public:
    COMMS_BITMASK_BITS(value);
};

class PublishFlags : public
    comms::field::Bitfield<
        FieldBase,
        std::tuple<
            SingleBitBitmask,
            QoS<comms::option::FixedBitLength<2> >,
            SingleBitBitmask,
            ReservedBits<comms::option::FixedBitLength<4> >
        >
    >
{
public:
    COMMS_FIELD_MEMBERS_ACCESS(retain, qos, dup, reserved);

    bool valid() const
    {
        // Mustn't have dup flag set when qos=0
        if (field_qos().value() != QosVal::AtMostOnceDelivery) {
            return true;
        }

        typedef typename std::decay<decltype(field_dup())>::type DupFieldType;
        if (field_dup().getBitValue(DupFieldType::BitIdx_value)) {
            return false;
        }

        using Base = typename std::decay<decltype(comms::field::toFieldBase(*this))>::type;
        return Base::valid();
    }
};

using PacketId =
    comms::field::IntValue<
        FieldBase,
        std::uint16_t,
        comms::option::DefaultNumValue<1>,
        comms::option::ValidNumValueRange<1, 0xffff>
    >;

using OptionalPacketId =
    comms::field::Optional<
        PacketId,
        comms::option::DefaultOptionalMode<comms::field::OptionalMode::Missing>
    >;

using Topic = String<>;

using Payload = BinData<>;

struct UnsubscribePayload : public
    comms::field::ArrayList<
        FieldBase,
        Topic
    >
{
    bool valid() const
    {
        using Base = typename std::decay<decltype(comms::field::toFieldBase(*this))>::type;
        return (!Base::value().empty()) && Base::valid();
    }
};


} // namespace field

} // namespace common

}  // namespace protocol

}  // namespace mqtt


