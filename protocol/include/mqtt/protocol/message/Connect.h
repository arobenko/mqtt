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
#include <algorithm>
#include "mqtt/protocol/Message.h"
#include "mqtt/protocol/field.h"

namespace mqtt
{

namespace protocol
{

namespace message
{

using ConnectFields = std::tuple<
    field::ProtocolName,
    field::ProtocolLevel,
    field::ConnectFlags,
    field::KeepAlive,
    field::ClientId,
    field::WillTopic,
    field::WillMessage,
    field::UserName,
    field::Password
>;

template <typename TMsgBase = Message>
class Connect : public
    comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_CONNECT>,
        comms::option::FieldsImpl<ConnectFields>,
        comms::option::DispatchImpl<Connect<TMsgBase> >,
        comms::option::NoDefaultFieldsReadImpl
    >
{
    typedef comms::MessageBase<
        TMsgBase,
        comms::option::StaticNumIdImpl<MsgId_CONNECT>,
        comms::option::FieldsImpl<ConnectFields>,
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
        typedef typename std::decay<decltype(flagsMembers.flagsLow)>::type FlagsLowField;
        typedef typename std::decay<decltype(flagsMembers.flagsHigh)>::type FlagsHighField;
        updateOptionalField(flagsMembers.flagsLow, FlagsLowField::BitIdx_willFlag, allFields.willTopic);
        updateOptionalField(flagsMembers.flagsLow, FlagsLowField::BitIdx_willFlag, allFields.willMessage);
        updateOptionalField(flagsMembers.flagsHigh, FlagsHighField::BitIdx_username, allFields.userName);
        updateOptionalField(flagsMembers.flagsHigh, FlagsHighField::BitIdx_password, allFields.password);

        return Base::template readFieldsFrom<FieldIdx_willTopic>(iter, size);
    }

    virtual bool refreshImpl() override
    {
        auto allFields = fieldsAsStruct();
        auto flagsMembers = allFields.flags.fieldsAsStruct();

        typedef typename std::decay<decltype(flagsMembers.flagsLow)>::type FlagsLowField;
        typedef typename std::decay<decltype(flagsMembers.flagsHigh)>::type FlagsHighField;

        bool updated = false;
        updated =
            refreshOptionalField(
                flagsMembers.flagsLow, FlagsLowField::BitIdx_willFlag, allFields.willTopic) || updated;
        updated =
            refreshOptionalField(
                flagsMembers.flagsLow, FlagsLowField::BitIdx_willFlag, allFields.willMessage) || updated;
        updated =
            refreshOptionalField(
                flagsMembers.flagsHigh, FlagsHighField::BitIdx_username, allFields.userName) || updated;
        updated =
            refreshOptionalField(
                flagsMembers.flagsHigh, FlagsHighField::BitIdx_password, allFields.password) || updated;

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

}  // namespace protocol

}  // namespace mqtt
