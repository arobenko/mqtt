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
#include "mqtt/protocol/v311/message/Pingreq.h"
#include "cc_plugin/protocol/Message.h"

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace message
{

class Pingreq : public
    comms_champion::ProtocolMessageBase<
        mqtt::protocol::v311::message::Pingreq<mqtt::cc_plugin::protocol::Message>,
        Pingreq>
{
public:
    Pingreq() = default;
    Pingreq(const Pingreq&) = default;
    Pingreq(Pingreq&&) = default;
    virtual ~Pingreq() = default;

    Pingreq& operator=(const Pingreq&) = default;
    Pingreq& operator=(Pingreq&&) = default;

protected:
    virtual const char* nameImpl() const override;
    virtual const QVariantList& fieldsPropertiesImpl() const override;
};

}  // namespace message

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqtt


