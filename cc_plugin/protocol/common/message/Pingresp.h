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

template <typename TMsgBase, typename TActual>
class Pingresp : public
    comms_champion::ProtocolMessageBase<TMsgBase, TActual>
{
public:
    Pingresp() = default;
    Pingresp(const Pingresp&) = default;
    Pingresp(Pingresp&&) = default;
    virtual ~Pingresp() = default;

    Pingresp& operator=(const Pingresp&) = default;
    Pingresp& operator=(Pingresp&&) = default;

protected:
    virtual const char* nameImpl() const override
    {
        return "PINGRESP";
    }

    virtual const QVariantList& fieldsPropertiesImpl() const override
    {
        return cc_plugin::protocol::common::field::emptyList();
    }
};

} // namespace message

} // namespace common

} // namespace protocol

} // namespace cc_plugin

} // namespace mqtt


