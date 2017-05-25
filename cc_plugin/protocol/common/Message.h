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

#include "comms_champion/comms_champion.h"

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace common
{

template <template<typename...> class TMsgBase>
class Message : public
    comms_champion::MessageBase<TMsgBase>
{
    using Base = comms_champion::MessageBase<TMsgBase>;
public:
    Message() = default;
    Message(const Message&) = default;
    Message(Message&&) = default;
    virtual ~Message() = default;
    Message& operator=(const Message&) = default;
    Message& operator=(Message&&) = default;
protected:
    virtual QString idAsStringImpl() const override
    {
        return QString("%1").arg(Base::getId(), 1, 10, QChar('0'));
    }
};

} // namespace common

} // namespace protocol

} // namespace cc_plugin

} // namespace mqtt
