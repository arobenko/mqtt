//
// Copyright 2016 (C). Alex Robenko. All rights reserved.
//

// This file is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "SocketConfigWidget.h"

#include <limits>
#include <algorithm>

namespace mqtt
{

namespace cc_plugin
{

namespace mosquitto_socket
{

namespace
{

void prepareQosCombo(QComboBox& box)
{
    box.addItem("0 (At most once delivery)");
    box.addItem("1 (At least once delivery)");
    box.addItem("2 (Exactly once delivery)");
}

}  // namespace

SocketConfigWidget::SocketConfigWidget(
    Socket& socket,
    QWidget* parentObj)
  : Base(parentObj),
    m_socket(socket)
{
    m_ui.setupUi(this);
    prepareQosCombo(*m_ui.m_subQosComboBox);
    prepareQosCombo(*m_ui.m_pubQosComboBox);

    m_ui.m_portSpinBox->setRange(
        1,
        static_cast<int>(std::numeric_limits<PortType>::max()));

    m_ui.m_hostLineEdit->setText(m_socket.getHost());

    m_ui.m_portSpinBox->setValue(
        static_cast<int>(m_socket.getPort()));

    m_ui.m_clientIdLineEdit->setText(m_socket.getId());

    m_ui.m_cleanCheckBox->setChecked(m_socket.getCleanSession());

    m_ui.m_keepAliveSpinBox->setValue(m_socket.getKeepAlivePeriod());

    m_ui.m_subTopicsLineEdit->setText(m_socket.getSubTopics());

    m_ui.m_subQosComboBox->setCurrentIndex(
        std::max(std::min(2, m_socket.getSubQos()), 0));

    m_ui.m_pubTopicLineEdit->setText(m_socket.getPubTopic());

    m_ui.m_pubQosComboBox->setCurrentIndex(
        std::max(std::min(2, m_socket.getPubQos()), 0));

    connect(
        m_ui.m_hostLineEdit, SIGNAL(textChanged(const QString&)),
        this, SLOT(hostValueChanged(const QString&)));

    connect(
        m_ui.m_portSpinBox, SIGNAL(valueChanged(int)),
        this, SLOT(portValueChanged(int)));

    connect(
        m_ui.m_clientIdLineEdit, SIGNAL(textChanged(const QString&)),
        this, SLOT(idChanged(const QString&)));

    connect(
        m_ui.m_cleanCheckBox, SIGNAL(stateChanged(int)),
        this, SLOT(cleanSessionChanged(int)));

    connect(
        m_ui.m_keepAliveSpinBox, SIGNAL(valueChanged(int)),
        this, SLOT(keepAliveChanged(int)));

    connect(
        m_ui.m_subTopicsLineEdit, SIGNAL(textChanged(const QString&)),
        this, SLOT(subTopicsChanged(const QString&)));

    connect(
        m_ui.m_subQosComboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(subQosChanged(int)));

    connect(
        m_ui.m_pubTopicLineEdit, SIGNAL(textChanged(const QString&)),
        this, SLOT(pubTopicChanged(const QString&)));

    connect(
        m_ui.m_pubQosComboBox, SIGNAL(currentIndexChanged(int)),
        this, SLOT(pubQosChanged(int)));
}

SocketConfigWidget::~SocketConfigWidget() = default;

void SocketConfigWidget::hostValueChanged(const QString& value)
{
    m_socket.setHost(value);
}

void SocketConfigWidget::portValueChanged(int value)
{
    m_socket.setPort(static_cast<PortType>(value));
}

void SocketConfigWidget::idChanged(const QString& value)
{
    m_socket.setId(value);
}

void SocketConfigWidget::cleanSessionChanged(int value)
{
    bool checked = (value != Qt::Unchecked);
    m_socket.setCleanSession(checked);
}

void SocketConfigWidget::keepAliveChanged(int value)
{
    m_socket.setKeepAlivePeriod(value);
}

void SocketConfigWidget::subTopicsChanged(const QString& value)
{
    m_socket.setSubTopics(value);
}

void SocketConfigWidget::subQosChanged(int value)
{
    m_socket.setSubQos(value);
}

void SocketConfigWidget::pubTopicChanged(const QString& value)
{
    m_socket.setPubTopic(value);
}

void SocketConfigWidget::pubQosChanged(int value)
{
    m_socket.setPubQos(value);
}


}  // namespace mosquitto_socket

}  // namespace cc_plugin

}  // namespace mqtt


