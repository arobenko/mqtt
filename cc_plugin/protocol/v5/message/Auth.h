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

#include "comms_champion/comms_champion.h"
#include "mqtt/protocol/v5/message/Auth.h"
#include "cc_plugin/protocol/v5/Message.h"

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace v5
{

namespace message
{

class Auth : public
    comms_champion::ProtocolMessageBase<
        mqtt::protocol::v5::message::Auth<mqtt::cc_plugin::protocol::v5::Message>,
        Auth>
{
public:
    Auth() = default;
    Auth(const Auth&) = default;
    Auth(Auth&&) = default;
    virtual ~Auth() = default;

    Auth& operator=(const Auth&) = default;
    Auth& operator=(Auth&&) = default;

protected:
    virtual const char* nameImpl() const override;
    virtual const QVariantList& fieldsPropertiesImpl() const override;
};

}  // namespace message

} // namespace v5

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqtt


