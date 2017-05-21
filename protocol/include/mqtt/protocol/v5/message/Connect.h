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

#include <tuple>
#include <algorithm>

#include "comms/MessageBase.h"
#include "mqtt/protocol/v5/field.h"
#include "mqtt/protocol/common/MsgId.h"

namespace mqtt
{

namespace protocol
{

namespace v5
{

namespace message
{

using ConnectFields = std::tuple<
    common::field::ProtocolName,
    v5::field::ProtocolVersion,
    common::field::ConnectFlags,
    common::field::KeepAlive,
    v5::field::Properties,
    common::field::ClientId,
    common::field::WillTopic,
    common::field::WillMessage,
    common::field::UserName,
    common::field::Password
>;

template <typename TMsgBase>
class Connect : public
        comms::MessageBase<
            TMsgBase,
            comms::option::StaticNumIdImpl<common::MsgId_CONNECT>,
            comms::option::FieldsImpl<ConnectFields>,
            comms::option::MsgType<Connect<TMsgBase> >,
            comms::option::HasDoRefresh
        >
{
public:
    COMMS_MSG_FIELDS_ACCESS(
        name,
        protVersion,
        flags,
        keepAlive,
        properties,
        clientId,
        willTopic,
        willMessage,
        userName,
        password);

    Connect() = default;
    Connect(const Connect&) = default;
    Connect(Connect&& other) = default;
    virtual ~Connect() = default;

    Connect& operator=(const Connect&) = default;
    Connect& operator=(Connect&&) = default;

    template <typename TIter>
    comms::ErrorStatus doRead(TIter& iter, std::size_t size)
    {
        using Base = typename std::decay<decltype(comms::toMessageBase(*this))>::type;
        auto status = Base::template readFieldsUntil<FieldIdx_willTopic>(iter, size);
        if (status != comms::ErrorStatus::Success) {
            return status;
        }

        auto& flagsLowField = field_flags().field_flagsLow();
        auto& flagsHighField = field_flags().field_flagsHigh();
        typedef typename std::decay<decltype(flagsLowField)>::type FlagsLowField;
        typedef typename std::decay<decltype(flagsHighField)>::type FlagsHighField;
        updateOptionalField(flagsLowField, FlagsLowField::BitIdx_willFlag, field_willTopic());
        updateOptionalField(flagsLowField, FlagsLowField::BitIdx_willFlag, field_willMessage());
        updateOptionalField(flagsHighField, FlagsHighField::BitIdx_username, field_userName());
        updateOptionalField(flagsHighField, FlagsHighField::BitIdx_password, field_password());

        return Base::template readFieldsFrom<FieldIdx_willTopic>(iter, size);
    }

    bool doRefresh()
    {
        auto& flagsLowField = field_flags().field_flagsLow();
        auto& flagsHighField = field_flags().field_flagsHigh();
        typedef typename std::decay<decltype(flagsLowField)>::type FlagsLowField;
        typedef typename std::decay<decltype(flagsHighField)>::type FlagsHighField;

        bool updated = false;
        updated =
            refreshOptionalField(
                flagsLowField, FlagsLowField::BitIdx_willFlag, field_willTopic()) || updated;
        updated =
            refreshOptionalField(
                flagsLowField, FlagsLowField::BitIdx_willFlag, field_willMessage()) || updated;
        updated =
            refreshOptionalField(
                flagsHighField, FlagsHighField::BitIdx_username, field_userName()) || updated;
        updated =
            refreshOptionalField(
                flagsHighField, FlagsHighField::BitIdx_password, field_password()) || updated;

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

} // namespace v5

}  // namespace protocol

}  // namespace mqtt
