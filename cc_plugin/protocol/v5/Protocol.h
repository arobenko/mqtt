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
#include "cc_plugin/protocol/v5/Stack.h"
#include "cc_plugin/protocol/v5/TransportMessage.h"

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace v5
{

class Protocol : public
    comms_champion::ProtocolBase<
        cc_plugin::protocol::v5::Stack,
        cc_plugin::protocol::v5::TransportMessage
    >
{
public:
    Protocol() = default;
    virtual ~Protocol();

protected:
    virtual const QString& nameImpl() const override;
};

} // namespace v5

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqtt


