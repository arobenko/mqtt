//
// Copyright 2016 (C). Alex Robenko. All rights reserved.
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

#include "comms/comms.h"

namespace mqtt
{

namespace protocol
{

namespace v311
{

namespace field
{

enum class SubackReturnCode : std::uint8_t
{
    SuccessQos0 = 0,
    SuccessQos1 = 1,
    SuccessQos2 = 2,
    Failure = 0x80
};

namespace details
{

struct ConnectFlagsExtraValidator
{
    template <typename TField>
    bool operator()(TField&& field) const
    {
        auto& members = field.value();
        auto& flagsLowField = std::get<0>(members);
        auto& willQosField = std::get<1>(members);
        auto& flagsHighField = std::get<2>(members);

        typedef typename std::decay<decltype(flagsLowField)>::type LowFlagsFieldType;
        typedef typename std::decay<decltype(willQosField)>::type QosFieldType;
        typedef typename std::decay<decltype(flagsHighField)>::type HighFlagsFieldType;

        if (!flagsLowField.getBitValue(LowFlagsFieldType::BitIdx_willFlag)) {
            if (willQosField.value() != QosFieldType::ValueType::AtMostOnceDelivery) {
                return false;
            }

            if (flagsHighField.getBitValue(HighFlagsFieldType::BitIdx_willRetain)) {
                return false;
            }
        }

        if (!flagsHighField.getBitValue(HighFlagsFieldType::BitIdx_username)) {
            if (flagsHighField.getBitValue(HighFlagsFieldType::BitIdx_password)) {
                return false;
            }
        }

        return true;
    }
};

struct ProtNameInitialiser
{
    template <typename TField>
    void operator()(TField&& field)
    {
        field.value() = "MQTT";
    }
};

struct ProtNameValidator
{
    template <typename TField>
    bool operator()(TField&& field)
    {
        return field.value() == "MQTT";
    }
};

struct PublishActualFlagsValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        auto& members = field.value();
        auto& qosField = std::get<1>(members);
        typedef typename std::decay<decltype(qosField)>::type QosFieldType;
        if (qosField.value() != QosFieldType::ValueType::AtMostOnceDelivery) {
            return true;
        }

        auto& dupField = std::get<2>(members);
        typedef typename std::decay<decltype(dupField)>::type DupFieldType;
        if (dupField.getBitValue(DupFieldType::BitIdx_value)) {
            return false;
        }
        return true;
    }
};


struct PublishTopicValidator
{
    template <typename TField>
    bool operator()(TField&& field) const
    {
        auto& topic = field.value();
        return
            (!topic.empty()) &&
            (std::none_of(
                topic.begin(), topic.end(),
                [](char ch) -> bool
                {
                    return (ch == '#') || (ch == '+');
                }));
    }
};

struct SubackPayloadValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        return 0U < field.value().size();
    }
};

struct SubackReturnCodeValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        auto value = field.value();
        return
            (value == SubackReturnCode::SuccessQos0) ||
            (value == SubackReturnCode::SuccessQos1) ||
            (value == SubackReturnCode::SuccessQos2) ||
            (value == SubackReturnCode::Failure);
    }
};

struct SubscribeTopicValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        auto& topic = field.value();
        return (!topic.empty());
    }
};

struct SubscribePayloadValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        return 0U < field.value().size();
    }
};

struct UnsubscribeTopicValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        auto& topic = field.value();
        return (!topic.empty());
    }
};

struct UnsubscribePayloadValidator
{
    template <typename TField>
    bool operator()(const TField& field) const
    {
        return 0U < field.value().size();
    }
};


}  // namespace details

using FieldBase = comms::Field<comms::option::BigEndian>;

using ConnackFlags =
    comms::field::BitmaskValue<
        FieldBase,
        comms::option::FixedLength<1>,
        comms::option::BitmaskReservedBits<0xfe, 0x0>
    >;

enum class ConnackResponseCodeVal : std::uint8_t
{
    Accepted,
    WrongProtocolVersion,
    IdentifierRejected,
    ServerUnavailable,
    BadUsernameOrPassword,
    NotAuthorized,
    NumOfValues
};

using ConnackResponseCode = comms::field::EnumValue<
        FieldBase,
        ConnackResponseCodeVal,
        comms::option::ValidNumValueRange<0, (int)(ConnackResponseCodeVal::NumOfValues) - 1>
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
        >,
        comms::option::ContentsValidator<details::ConnectFlagsExtraValidator>
    >
{
public:
    COMMS_FIELD_MEMBERS_ACCESS(flagsLow, willQos, flagsHigh);
};

using ProtocolName =
    comms::field::String<
        FieldBase,
        comms::option::DefaultValueInitialiser<details::ProtNameInitialiser>,
        comms::option::ContentsValidator<details::ProtNameValidator>,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<FieldBase, std::uint16_t>
        >
    >;

using ProtocolLevel =
    comms::field::IntValue<
        FieldBase,
        std::uint8_t,
        comms::option::DefaultNumValue<4>,
        comms::option::ValidNumValueRange<4, 4>
    >;

using KeepAlive = comms::field::IntValue<FieldBase, std::uint16_t>;

using ClientId =
    comms::field::String<
        FieldBase,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<
                FieldBase,
                std::uint16_t,
                comms::option::ValidNumValueRange<0, 23>
            >
        >
    >;

using WillTopic =
    comms::field::Optional<
        comms::field::String<
            FieldBase,
            comms::option::SequenceSizeFieldPrefix<
                comms::field::IntValue<
                    FieldBase,
                    std::uint16_t
                >
            >
        >
    >;


using WillMessage =
    comms::field::Optional<
        comms::field::ArrayList<
            FieldBase,
            std::uint8_t,
            comms::option::SequenceSizeFieldPrefix<
                comms::field::IntValue<
                    FieldBase,
                    std::uint16_t
                >
            >
        >
    >;

using UserName =
    comms::field::Optional<
        comms::field::String<
            FieldBase,
            comms::option::SequenceSizeFieldPrefix<
                comms::field::IntValue<
                    FieldBase,
                    std::uint16_t
                >
            >
        >
    >;

using Password =
    comms::field::Optional<
        comms::field::ArrayList<
            FieldBase,
            std::uint8_t,
            comms::option::SequenceSizeFieldPrefix<
                comms::field::IntValue<
                    FieldBase,
                    std::uint16_t
                >
            >
        >
    >;

using PacketId =
    comms::field::IntValue<
        FieldBase,
        std::uint16_t,
        comms::option::DefaultNumValue<1>,
        comms::option::ValidNumValueRange<1, 0xffff>
    >;

using OptionalPacketId = comms::field::Optional<PacketId>;

class PubSingleBitFlag :
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
            PubSingleBitFlag,
            QoS<comms::option::FixedBitLength<2> >,
            field::PubSingleBitFlag,
            comms::field::IntValue<FieldBase, std::uint8_t, comms::option::FixedBitLength<4> >
        >,
        comms::option::ContentsValidator<details::PublishActualFlagsValidator>
    >
{
public:
    COMMS_FIELD_MEMBERS_ACCESS(retain, qos, dup, reserved);
};


using PublishTopic =
    comms::field::String<
        FieldBase,
        comms::option::ContentsValidator<details::PublishTopicValidator>,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<
                FieldBase,
                std::uint16_t
            >
        >
    >;

using Payload =
    comms::field::ArrayList<FieldBase, std::uint8_t>;


using SubackPayload =
    comms::field::ArrayList<
        FieldBase,
        comms::field::EnumValue<
            FieldBase,
            SubackReturnCode,
            comms::option::ContentsValidator<details::SubackReturnCodeValidator>
        >,
        comms::option::ContentsValidator<details::SubackPayloadValidator>
    >;

using SubscribeTopic =
    comms::field::String<
        FieldBase,
        comms::option::ContentsValidator<details::SubscribeTopicValidator>,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<
                FieldBase,
                std::uint16_t
            >
        >
    >;

using SubElemBase =
    comms::field::Bundle<
        FieldBase,
        std::tuple<
            SubscribeTopic,
            QoS<>
        >
    >;

class SubElem : public
    comms::field::Bundle<
        FieldBase,
        std::tuple<
            SubscribeTopic,
            QoS<>
        >
    >
{
public:
    COMMS_FIELD_MEMBERS_ACCESS(topic, qos);
};

using SubscribePayload =
    comms::field::ArrayList<
        FieldBase,
        SubElem,
        comms::option::ContentsValidator<details::SubscribePayloadValidator>
    >;

using UnsubscribeTopic =
    comms::field::String<
        FieldBase,
        comms::option::ContentsValidator<details::UnsubscribeTopicValidator>,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<
                FieldBase,
                std::uint16_t
            >
        >
    >;

using UnsubscribePayload =
    comms::field::ArrayList<
        FieldBase,
        UnsubscribeTopic,
        comms::option::ContentsValidator<details::UnsubscribePayloadValidator>
    >;

}  // namespace field

} // namespace v311

}  // namespace protocol

}  // namespace mqtt


