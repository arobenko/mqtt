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

#include <cstdint>

namespace mqtt
{

enum MsgId : std::uint8_t
{
    MsgId_Reserved,
    MsgId_CONNECT,
    MsgId_CONNACK,
    MsgId_PUBLISH,
    MsgId_PUBACK,
    MsgId_PUBREC,
    MsgId_PUBREL,
    MsgId_PUBCOMP,
    MsgId_SUBSCRIBE,
    MsgId_NumOfValues // Mast be last
};

}  // namespace mqtt


