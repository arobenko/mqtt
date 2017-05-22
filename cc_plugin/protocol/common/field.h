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

#include <QtCore/QVariantList>
#include <QtCore/QVariantMap>

#include "mqtt/protocol/common/field.h"

namespace mqtt
{

namespace cc_plugin
{

namespace protocol
{

namespace common
{

namespace field
{

QVariantList createProps_transportFields(mqtt::protocol::common::field::ProtocolVersionVal version);
const QVariantList& emptyList();
QVariantList createProps_connect(mqtt::protocol::common::field::ProtocolVersionVal version);
QVariantList createProps_connack(mqtt::protocol::common::field::ProtocolVersionVal version);
QVariantList createProps_publish(mqtt::protocol::common::field::ProtocolVersionVal version);
QVariantList createProps_puback(mqtt::protocol::common::field::ProtocolVersionVal version);
QVariantList createProps_pubrec(mqtt::protocol::common::field::ProtocolVersionVal version);
QVariantList createProps_pubrel(mqtt::protocol::common::field::ProtocolVersionVal version);
QVariantList createProps_pubcomp(mqtt::protocol::common::field::ProtocolVersionVal version);

} // namespace field

} // namespace common

} // namespace protocol

} // namespace cc_plugin

} // namespace mqtt
