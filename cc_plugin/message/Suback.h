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

#include "comms_champion/comms_champion.h"
#include "mqtt/message/Suback.h"
#include "cc_plugin/Message.h"

namespace mqtt
{

namespace cc_plugin
{

namespace message
{

class Suback : public
    comms_champion::ProtocolMessageBase<
        mqtt::message::Suback<mqtt::cc_plugin::Message>,
        Suback>
{
public:
    Suback() = default;
    Suback(const Suback&) = default;
    Suback(Suback&&) = default;
    virtual ~Suback() = default;

    Suback& operator=(const Suback&) = default;
    Suback& operator=(Suback&&) = default;

protected:
    virtual const char* nameImpl() const override;
    virtual const QVariantList& fieldsPropertiesImpl() const override;
};

}  // namespace message

}  // namespace cc_plugin

}  // namespace mqtt


