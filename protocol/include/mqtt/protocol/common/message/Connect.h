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
#include "mqtt/protocol/common/MsgId.h"

namespace mqtt
{

namespace protocol
{

namespace common
{

namespace message
{

template <typename TMsgBase, typename TAllfields, typename TActual>
class Connect : public
        comms::MessageBase<
            TMsgBase,
            comms::option::StaticNumIdImpl<MsgId_CONNECT>,
            comms::option::FieldsImpl<TAllfields>,
            comms::option::MsgType<TActual>,
            comms::option::HasDoRefresh
        >
{
public:
    template <typename TIter>
    comms::ErrorStatus doRead(TIter& iter, std::size_t size)
    {
        using Base = typename std::decay<decltype(comms::toMessageBase(*this))>::type;
        auto status = Base::template doReadUntil<TActual::FieldIdx_willTopic>(iter, size);
        if (status != comms::ErrorStatus::Success) {
            return status;
        }

        auto& actual = static_cast<TActual&>(*this);
        auto& flagsLowField = actual.field_flags().field_flagsLow();
        auto& flagsHighField = actual.field_flags().field_flagsHigh();
        using FlagsLowField = typename std::decay<decltype(flagsLowField)>::type;
        using FlagsHighField = typename std::decay<decltype(flagsHighField)>::type;

        updateOptionalField(flagsLowField, FlagsLowField::BitIdx_willFlag, actual.field_willTopic());
        updateOptionalField(flagsLowField, FlagsLowField::BitIdx_willFlag, actual.field_willMessage());
        updateOptionalField(flagsHighField, FlagsHighField::BitIdx_username, actual.field_userName());
        updateOptionalField(flagsHighField, FlagsHighField::BitIdx_password, actual.field_password());

        return Base::template doReadFrom<TActual::FieldIdx_willTopic>(iter, size);
    }

    bool doRefresh()
    {
        auto& actual = static_cast<TActual&>(*this);
        auto& flagsLowField = actual.field_flags().field_flagsLow();
        auto& flagsHighField = actual.field_flags().field_flagsHigh();
        using FlagsLowField = typename std::decay<decltype(flagsLowField)>::type;
        using FlagsHighField = typename std::decay<decltype(flagsHighField)>::type;

        bool updated = false;
        updated =
            refreshOptionalField(
                flagsLowField, FlagsLowField::BitIdx_willFlag, actual.field_willTopic()) || updated;
        updated =
            refreshOptionalField(
                flagsLowField, FlagsLowField::BitIdx_willFlag, actual.field_willMessage()) || updated;
        updated =
            refreshOptionalField(
                flagsHighField, FlagsHighField::BitIdx_username, actual.field_userName()) || updated;
        updated =
            refreshOptionalField(
                flagsHighField, FlagsHighField::BitIdx_password, actual.field_password()) || updated;

        return updated;
    }

protected:
    Connect() = default;
    Connect(const Connect&) = default;
    Connect(Connect&& other) = default;
    ~Connect() = default;

    Connect& operator=(const Connect&) = default;
    Connect& operator=(Connect&&) = default;

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

} // namespace message

} // namespace common

} // namespace protocol

} // namespace mqtt
