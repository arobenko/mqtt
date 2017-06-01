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
#include "cc_plugin/protocol/common/field.h"

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace common
{

namespace message
{

template <
    mqtt::protocol::common::field::ProtocolVersionVal TVer,
    typename TMsgBase,
    typename TActual>
class Connack : public
    comms_champion::ProtocolMessageBase<TMsgBase, TActual>
{
public:
    Connack() = default;
    Connack(const Connack&) = default;
    Connack(Connack&&) = default;
    virtual ~Connack() = default;

    Connack& operator=(const Connack&) = default;
    Connack& operator=(Connack&&) = default;

protected:
    virtual const char* nameImpl() const override
    {
        return "CONNACK";
    }

    virtual const QVariantList& fieldsPropertiesImpl() const override
    {
        static const QVariantList Props =
                cc_plugin::protocol::common::field::createProps_connack(TVer);
        assert(Props.size() == TMsgBase::FieldIdx_numOfValues);
        return Props;
    }
};

} // namespace message

} // namespace common

} // namespace protocol

} // namespace cc_plugin

} // namespace mqtt


