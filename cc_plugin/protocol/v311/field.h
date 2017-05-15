//
// Copyright 2016 (C). Alex Robenko. All rights reserved.
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

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtCore/QObject>
#include <QtCore/QString>
CC_ENABLE_WARNINGS()

#include "comms_champion/comms_champion.h"

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace v311
{

namespace field
{

const QString& packetIdFieldName();
const QVariantMap& packetIdProperties();
const QVariantMap& optionalPacketIdProperties();

void updateQosPropertiesMap(comms_champion::property::field::EnumValue& props);

}  // namespace field

} // namespace v311

}  // namespace protocol

}  // namespace cc_plugin

}  // namespace mqtt





