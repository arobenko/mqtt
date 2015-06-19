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

#include <tuple>
#include <algorithm>
#include "mqtt/Message.h"
#include "mqtt/field/QoS.h"

namespace mqtt
{

namespace message
{

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

template <typename TFieldBase>
using ProtocolNameField =
    comms::field::String<
        TFieldBase,
        comms::option::DefaultValueInitialiser<ProtNameInitialiser>,
        comms::option::ContentsValidator<ProtNameValidator>,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<
                TFieldBase,
                std::uint16_t
            >
        >
    >;

template <typename TFieldBase>
using ProtocolLevelField =
    comms::field::IntValue<
        TFieldBase,
        std::uint8_t,
        comms::option::DefaultNumValue<4>,
        comms::option::ValidNumValueRange<4, 4>
    >;

enum ConnectFlagsMemberIdx
{
    ConnectFlagsMemberIdx_FlagsLow,
    ConnectFlagsMemberIdx_WillQos,
    ConnectFlagsMemberIdx_FlagsHigh
};

enum ConnectFlagsLowBitIdx
{
    ConnectFlagsLowBitIdx_Reserved,
    ConnectFlagsLowBitIdx_CleanSession,
    ConnectFlagsLowBitIdx_WillFlag
};

enum ConnectFlagsHighBitIdx
{
    ConnectFlagsHighBitIdx_WillRetain,
    ConnectFlagsHighBitIdx_PasswordFlag,
    ConnectFlagsHighBitIdx_UserNameFlag
};

struct ConnectFlagsExtraValidator
{
    template <typename TField>
    bool operator()(TField&& field) const
    {
        auto& members = field.value();
        auto& flagsLowField = std::get<ConnectFlagsMemberIdx_FlagsLow>(members);
        auto& willQosField = std::get<ConnectFlagsMemberIdx_WillQos>(members);
        auto& flagsHighField = std::get<ConnectFlagsMemberIdx_FlagsHigh>(members);

        if (!flagsLowField.getBitValue(ConnectFlagsLowBitIdx_WillFlag)) {
            if (willQosField.value() != mqtt::field::QosType::AtMostOnceDelivery) {
                return false;
            }

            if (flagsHighField.getBitValue(ConnectFlagsHighBitIdx_WillRetain)) {
                return false;
            }
        }

        if (!flagsHighField.getBitValue(ConnectFlagsHighBitIdx_UserNameFlag)) {
            if (flagsHighField.getBitValue(ConnectFlagsHighBitIdx_PasswordFlag)) {
                return false;
            }
        }

        return true;
    }
};

template <typename TFieldBase>
using ConnectFlagsField =
    comms::field::Bitfield<
        TFieldBase,
        std::tuple<
            comms::field::BitmaskValue<
                TFieldBase,
                comms::option::FixedBitLength<3>,
                comms::option::BitmaskReservedBits<0x1, 0>
            >,
            mqtt::field::QoS<
                TFieldBase,
                comms::option::FixedBitLength<2>
            >,
            comms::field::BitmaskValue<
                TFieldBase,
                comms::option::FixedBitLength<3>
            >
        >,
        comms::option::ContentsValidator<ConnectFlagsExtraValidator>
    >;

template <typename TFieldBase>
using KeepAliveField =
    comms::field::IntValue<
        TFieldBase,
        std::uint16_t
    >;

template <typename TFieldBase>
using ClientIdField =
    comms::field::String<
        TFieldBase,
        comms::option::SequenceSizeFieldPrefix<
            comms::field::IntValue<
                TFieldBase,
                std::uint16_t,
                comms::option::ValidNumValueRange<1, 23>
            >
        >
    >;

template <typename TFieldBase>
using WillTopicField =
    comms::field::Optional<
        comms::field::String<
            TFieldBase,
            comms::option::SequenceSizeFieldPrefix<
                comms::field::IntValue<
                    TFieldBase,
                    std::uint16_t
                >
            >
        >
    >;


template <typename TFieldBase>
using WillMessageField =
    comms::field::Optional<
        comms::field::ArrayList<
            TFieldBase,
            std::uint8_t,
            comms::option::SequenceSizeFieldPrefix<
                comms::field::IntValue<
                    TFieldBase,
                    std::uint16_t
                >
            >
        >
    >;

template <typename TFieldBase>
using UserNameField =
    comms::field::Optional<
        comms::field::String<
            TFieldBase,
            comms::option::SequenceSizeFieldPrefix<
                comms::field::IntValue<
                    TFieldBase,
                    std::uint16_t
                >
            >
        >
    >;

template <typename TFieldBase>
using PasswordField =
    comms::field::Optional<
        comms::field::ArrayList<
            TFieldBase,
            std::uint8_t,
            comms::option::SequenceSizeFieldPrefix<
                comms::field::IntValue<
                    TFieldBase,
                    std::uint16_t
                >
            >
        >
    >;

template <typename TFieldBase>
using ConnectFields = std::tuple<
    ProtocolNameField<TFieldBase>,
    ProtocolLevelField<TFieldBase>,
    ConnectFlagsField<TFieldBase>,
    KeepAliveField<TFieldBase>,
    ClientIdField<TFieldBase>,
    WillTopicField<TFieldBase>,
    WillMessageField<TFieldBase>,
    UserNameField<TFieldBase>,
    PasswordField<TFieldBase>
>;

template <typename TMsgBase = Message>
class Connect : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_CONNECT>,
        comms::option::FieldsImpl<ConnectFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Connect<TMsgBase> >
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_CONNECT>,
        comms::option::FieldsImpl<ConnectFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Connect<TMsgBase> >
    > Base;
public:
    enum FieldIdx
    {
        FieldIdx_Name,
        FieldIdx_Level,
        FieldIdx_Flags,
        FieldIdx_KeepAlive,
        FieldIdx_ClientId,
        FieldIdx_WillTopic,
        FieldIdx_WillMessage,
        FieldIdx_UserName,
        FieldIdx_Password,
        FieldIdx_NumOfValues
    };

    static_assert(std::tuple_size<typename Base::AllFields>::value == FieldIdx_NumOfValues,
        "Number of fields is incorrect");

    Connect()
    {
        auto& fields = Base::getFields();
        auto& willMsgField = std::get<FieldIdx_WillMessage>(fields);

        willMsgField.setMode(comms::field::OptionalMode::Missing);
    }

    Connect(const Connect&) = default;
    Connect(Connect&& other)
    {
    }
    virtual ~Connect() = default;

    Connect& operator=(const Connect&) = default;
    Connect& operator=(Connect&&) = default;

protected:
    virtual comms::ErrorStatus readImpl(
        typename Base::ReadIterator& iter,
        std::size_t size) override
    {
        auto status = Base::template readFieldsUntil<FieldIdx_WillMessage>(iter, size);
        if (status != comms::ErrorStatus::Success) {
            return status;
        }

        auto& fields = Base::getFields();
        auto& flagsField = std::get<FieldIdx_Flags>(fields);
        auto& willTopicField = std::get<FieldIdx_WillTopic>(fields);
        auto& willMessageField = std::get<FieldIdx_WillMessage>(fields);

        auto& flagsMembers = flagsField.value();
        auto& flagsLowMember = std::get<ConnectFlagsMemberIdx_FlagsLow>(flagsMembers);
        updateOptionalField(flagsLowMember, ConnectFlagsLowBitIdx_WillFlag, willTopicField);
        updateOptionalField(flagsLowMember, ConnectFlagsLowBitIdx_WillFlag, willMessageField);

        auto& flagsHighMember = std::get<ConnectFlagsMemberIdx_FlagsHigh>(flagsMembers);

        auto& userNameField = std::get<FieldIdx_UserName>(fields);
        auto& passwordField = std::get<FieldIdx_Password>(fields);
        updateOptionalField(flagsHighMember, ConnectFlagsHighBitIdx_UserNameFlag, userNameField);
        updateOptionalField(flagsHighMember, ConnectFlagsHighBitIdx_PasswordFlag, passwordField);

        return Base::template readFieldsFrom<FieldIdx_WillMessage>(iter, size);
    }

    virtual bool refreshImpl() override
    {
        auto& fields = Base::getFields();
        auto& flagsField = std::get<FieldIdx_Flags>(fields);
        auto& willTopicField = std::get<FieldIdx_WillTopic>(fields);
        auto& willMessageField = std::get<FieldIdx_WillMessage>(fields);
        auto& userNameField = std::get<FieldIdx_UserName>(fields);
        auto& passwordField = std::get<FieldIdx_Password>(fields);

        auto& flagsMembers = flagsField.value();
        auto& flagsLowMember = std::get<ConnectFlagsMemberIdx_FlagsLow>(flagsMembers);
        auto& flagsHighMember = std::get<ConnectFlagsMemberIdx_FlagsHigh>(flagsMembers);

        bool updated = false;
        updated =
            refreshOptionalField(
                flagsLowMember, ConnectFlagsLowBitIdx_WillFlag, willTopicField) || updated;
        updated =
            refreshOptionalField(
                flagsLowMember, ConnectFlagsLowBitIdx_WillFlag, willMessageField) || updated;
        updated =
            refreshOptionalField(
                flagsHighMember, ConnectFlagsHighBitIdx_UserNameFlag, userNameField) || updated;
        updated =
            refreshOptionalField(
                flagsHighMember, ConnectFlagsHighBitIdx_PasswordFlag, passwordField) || updated;

        return updated;
    }

private:
    template <typename TFlagsField, typename TOptField>
    static void updateOptionalField(const TFlagsField& flagsField, unsigned idx, TOptField& optField)
    {
        if (flagsField.getBitValue(idx)) {
            optField.setMode(comms::field::OptionalMode::Exists);
        }
        else {
            optField.setMode(comms::field::OptionalMode::Missing);
        }
    }

    template <typename TFlagsField, typename TOptField>
    static bool refreshOptionalField(const TFlagsField& flagsField, unsigned idx, TOptField& optField)
    {
        bool updated = false;
        if (flagsField.getBitValue(idx)) {
            updated = (optField.getMode() != comms::field::OptionalMode::Exists);
            optField.setMode(comms::field::OptionalMode::Exists);
        }
        else {
            updated = (optField.getMode() != comms::field::OptionalMode::Missing);
            optField.setMode(comms::field::OptionalMode::Missing);
        }
        return updated;
    }
};

}  // namespace message


}  // namespace mqtt
