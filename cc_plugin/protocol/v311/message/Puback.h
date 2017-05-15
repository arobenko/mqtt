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

#include "comms_champion/comms_champion.h"
#include "mqtt/protocol/v311/message/Puback.h"
#include "cc_plugin/protocol/v311/Message.h"

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace v311
{

namespace message
{

class Puback : public
    comms_champion::ProtocolMessageBase<
        mqtt::protocol::v311::message::Puback<mqtt::cc_plugin::protocol::v311::Message>,
        Puback>
{
public:
    Puback() = default;
    Puback(const Puback&) = default;
    Puback(Puback&&) = default;
    virtual ~Puback() = default;

    Puback& operator=(const Puback&) = default;
    Puback& operator=(Puback&&) = default;

protected:
    virtual const char* nameImpl() const override;
    virtual const QVariantList& fieldsPropertiesImpl() const override;
};

}  // namespace message

} // namespace v311

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqtt


