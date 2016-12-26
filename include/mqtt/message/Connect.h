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
using ConnectFlagsFieldBase =
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

}  // namespace details

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

template <typename TFieldBase>
class ConnectFlagsField : public details::ConnectFlagsFieldBase<TFieldBase>
{
    typedef details::ConnectFlagsFieldBase<TFieldBase> Base;
public:
    COMMS_FIELD_MEMBERS_ACCESS(Base, flagsLow, willQos, flagsHigh);
};

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
                comms::option::ValidNumValueRange<0, 23>
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
        comms::option::DispatchImpl<Connect<TMsgBase> >,
        comms::option::NoDefaultFieldsReadImpl
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_CONNECT>,
        comms::option::FieldsImpl<ConnectFields<typename TMsgBase::Field> >,
        comms::option::DispatchImpl<Connect<TMsgBase> >,
        comms::option::NoDefaultFieldsReadImpl
    > Base;
public:
    COMMS_MSG_FIELDS_ACCESS(Base, name, level, flags, keepAlive, clientId, willTopic, willMessage, userName, password);

    Connect()
    {
        auto& allFields = Base::fields();
        auto& willMsgField = std::get<FieldIdx_willMessage>(allFields);

        willMsgField.setMode(comms::field::OptionalMode::Missing);
    }

    Connect(const Connect&) = default;
    Connect(Connect&& other) = default;
    virtual ~Connect() = default;

    Connect& operator=(const Connect&) = default;
    Connect& operator=(Connect&&) = default;

protected:
    virtual comms::ErrorStatus readImpl(
        typename Base::ReadIterator& iter,
        std::size_t size) override
    {
        auto status = Base::template readFieldsUntil<FieldIdx_willTopic>(iter, size);
        if (status != comms::ErrorStatus::Success) {
            return status;
        }

        auto allFields = fieldsAsStruct();
        auto flagsMembers = allFields.flags.fieldsAsStruct();
        updateOptionalField(flagsMembers.flagsLow, ConnectFlagsLowBitIdx_WillFlag, allFields.willTopic);
        updateOptionalField(flagsMembers.flagsLow, ConnectFlagsLowBitIdx_WillFlag, allFields.willMessage);
        updateOptionalField(flagsMembers.flagsHigh, ConnectFlagsHighBitIdx_UserNameFlag, allFields.userName);
        updateOptionalField(flagsMembers.flagsHigh, ConnectFlagsHighBitIdx_PasswordFlag, allFields.password);

        return Base::template readFieldsFrom<FieldIdx_willTopic>(iter, size);
    }

    virtual bool refreshImpl() override
    {
        auto allFields = fieldsAsStruct();
        auto flagsMembers = allFields.flags.fieldsAsStruct();

        bool updated = false;
        updated =
            refreshOptionalField(
                flagsMembers.flagsLow, ConnectFlagsLowBitIdx_WillFlag, allFields.willTopic) || updated;
        updated =
            refreshOptionalField(
                flagsMembers.flagsLow, ConnectFlagsLowBitIdx_WillFlag, allFields.willMessage) || updated;
        updated =
            refreshOptionalField(
                flagsMembers.flagsHigh, ConnectFlagsHighBitIdx_UserNameFlag, allFields.userName) || updated;
        updated =
            refreshOptionalField(
                flagsMembers.flagsHigh, ConnectFlagsHighBitIdx_PasswordFlag, allFields.password) || updated;

        return updated;
    }

private:
    template <typename TFlagsField, typename TOptField>
    static void updateOptionalField(const TFlagsField& flagsField, unsigned idx, TOptField& optField)
    {
        if (flagsField.getBitValue(idx)) {
            optField.setExists();
        }
        else {
            optField.setMissing();
        }
    }

    template <typename TFlagsField, typename TOptField>
    static bool refreshOptionalField(const TFlagsField& flagsField, unsigned idx, TOptField& optField)
    {
        bool updated = false;
        if (flagsField.getBitValue(idx)) {
            updated = !optField.doesExist();
            optField.setExists();
        }
        else {
            updated = !optField.isMissing();
            optField.setMissing();
        }
        return updated;
    }
};

}  // namespace message


}  // namespace mqtt
