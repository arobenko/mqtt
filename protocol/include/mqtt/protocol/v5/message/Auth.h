//
// Copyright 2017 (C). Alex Robenko. All rights reserved.
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
#include "comms/MessageBase.h"
#include "mqtt/protocol/common/MsgId.h"
#include "mqtt/protocol/v5/field.h"

namespace mqtt
{

namespace protocol
{

namespace v5
{

namespace message
{

using AuthFields = std::tuple<
    v5::field::ResponseCode,
    v5::field::Properties
>;

template <typename TMsgBase>
class Auth : public
        comms::MessageBase<
            TMsgBase,
            comms::option::StaticNumIdImpl<common::MsgId_AUTH>,
            comms::option::FieldsImpl<AuthFields>,
            comms::option::MsgType<Auth<TMsgBase> >
        >
{
    using AuthFlagsField = comms::field::IntValue<
        common::field::FieldBase,
        std::uint8_t,
        comms::option::DefaultNumValue<1>,
        comms::option::ValidNumValueRange<1, 1>,
        comms::option::FailOnInvalid<comms::ErrorStatus::ProtocolError>
    >;

public:
    COMMS_MSG_FIELDS_ACCESS(responseCode, properties);

    Auth()
    {
        using Base = typename std::decay<decltype(comms::toMessageBase(*this))>::type;
        using FlagsField = typename Base::FlagsField;

        FlagsField newFlags(AuthFlagsField().value());
        Base::setFlags(newFlags);
    }

    Auth(const Auth&) = default;
    Auth(Auth&& other) = default;
    ~Auth() = default;

    Auth& operator=(const Auth&) = default;
    Auth& operator=(Auth&&) = default;

    bool doValid() const
    {
        using Base = typename std::decay<decltype(comms::toMessageBase(*this))>::type;
        auto& flagsField = Base::getFlags();
        AuthFlagsField actFlags(flagsField.value());

        return actFlags.valid() && Base::doValid();
    }

};

} // namespace message

} // namespace v5

} // namespace protocol

} // namespace mqtt
