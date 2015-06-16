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

#include <type_traits>
#include <functional>
#include <cassert>

#include "Pingresp.h"

#include "cc_plugin/field/PacketId.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace message
{

const char* Pingresp::nameImpl() const
{
    static const char* Str = "PINGRESP";
    return Str;
}

const QVariantList& Pingresp::fieldsPropertiesImpl() const
{
    static const QVariantList Props;
    return Props;
}


}  // namespace message

}  // namespace cc_plugin

}  // namespace mqtt

