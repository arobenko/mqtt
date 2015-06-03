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

#include <QtCore/QVariantMap>

#include "cc_plugin/field/QoS.h"
#include "cc_plugin/field/PacketId.h"
#include "Publish.h"

namespace cc = comms_champion;

namespace mqtt
{

namespace cc_plugin
{

namespace message
{

namespace
{

const QString& getEmptyString()
{
    static const QString Str;
    return Str;
}

QVariantMap getRetainMemberData()
{
    QVariantMap map;
    static const QString Name("Flags");
    map.insert(cc::Property::name(), QVariant::fromValue(Name));

    static const QString Map[] = {
        "RETAIN"
    };

    static const unsigned MapSize = std::extent<decltype(Map)>::value;

    for (auto idx = 0U; idx < MapSize; ++idx) {
        map.insert(cc::Property::indexedName(idx), QVariant::fromValue(Map[idx]));
    }
    return map;
}

QVariantMap getQosMemberData()
{
    QVariantMap map;
    static const QString Name("QoS");
    map.insert(cc::Property::name(), QVariant::fromValue(Name));
    field::updateQosPropertiesMap(map);
    return map;
}

QVariantMap getDupMemberData()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(getEmptyString()));

    static const QString Map[] = {
        "DUP"
    };

    static const unsigned MapSize = std::extent<decltype(Map)>::value;

    for (auto idx = 0U; idx < MapSize; ++idx) {
        map.insert(cc::Property::indexedName(idx), QVariant::fromValue(Map[idx]));
    }
    return map;
}

QVariantMap getReservedMemberData()
{
    QVariantMap map;
    map.insert(cc::Property::name(), QVariant::fromValue(getEmptyString()));
    map.insert(cc::Property::fieldHidden(), QVariant::fromValue(true));
    return map;
}


void updateTopicProperties(QObject& fieldWidget)
{
    static const QString Str("Topic");
    cc::Property::setNameVal(fieldWidget, Str);
}

void updatePayloadProperties(QObject& fieldWidget)
{
    static const QString Str("Payload");
    cc::Property::setNameVal(fieldWidget, Str);
}

}  // namespace

Publish::Publish()
  : m_actFlags(getFlags().getValue())
{
}


const char* Publish::nameImpl() const
{
    static const char* Str = "PUBLISH";
    return Str;
}

void Publish::widgetCreationEndNotificationImpl(cc::MessageWidget& widget)
{
    auto* castedWidget = qobject_cast<cc::DefaultMessageWidget*>(&widget);
    if (castedWidget == nullptr) {
        assert(!"Something is wrong");
        return;
    }

    auto actFlagsWidget = cc::FieldWidgetCreator::createWidget(m_actFlags);
    assert(actFlagsWidget);
    m_actFlagsWidget = actFlagsWidget.get();

    static const QString Str("Flags");
    cc::Property::setNameVal(*m_actFlagsWidget, Str);
    cc::Property::setSerialisedHiddenVal(*m_actFlagsWidget, true);
    cc::Property::setIndexedDataVal(
        *actFlagsWidget, mqtt::message::PublishActualFlagIdx_Retain, getRetainMemberData());
    cc::Property::setIndexedDataVal(
        *actFlagsWidget, mqtt::message::PublishActualFlagIdx_QoS, getQosMemberData());
    cc::Property::setIndexedDataVal(
        *actFlagsWidget, mqtt::message::PublishActualFlagIdx_Dup, getDupMemberData());
    cc::Property::setIndexedDataVal(
        *actFlagsWidget, mqtt::message::PublishActualFlagIdx_Reserved, getReservedMemberData());

    actFlagsWidget->propertiesUpdated();

    connect(
        m_actFlagsWidget, SIGNAL(sigFieldUpdated()),
        this, SLOT(flagsUpdated()));

    castedWidget->insertFieldWidget(0, actFlagsWidget.release());
}

void Publish::updateFieldPropertiesImpl(QWidget& fieldWidget, uint idx) const
{
    typedef std::function<void (QObject&)> FieldUpdateFunc;
    static const FieldUpdateFunc FuncMap[] = {
        &updateTopicProperties,
        &cc_plugin::field::updateOptionalPacketIdProperties,
        &updatePayloadProperties
    };

    static const unsigned FuncsCount = std::extent<decltype(FuncMap)>::value;

    static_assert(FuncsCount == FieldIdx_NumOfValues,
        "The funcs map is incorrect");

    if (FuncsCount <= idx) {
        return;
    }

    assert(FuncMap[idx]);
    FuncMap[idx](fieldWidget);
}

void Publish::flagsUpdated()
{
    setFlags(FlagsField(m_actFlags.getValue()));
}


}  // namespace message

}  // namespace cc_plugin

}  // namespace mqtt

