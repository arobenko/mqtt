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


#pragma once

#include "comms/CompileControl.h"

CC_DISABLE_WARNINGS()
#include <QtWidgets/QWidget>
#include "ui_SocketConfigWidget.h"
CC_ENABLE_WARNINGS()

#include "Socket.h"

namespace mqtt
{

namespace cc_plugin
{

namespace mosquitto_socket
{

class SocketConfigWidget : public QWidget
{
    Q_OBJECT
    typedef QWidget Base;
public:
    typedef Socket::PortType PortType;

    explicit SocketConfigWidget(
        Socket& socket,
        QWidget* parentObj = nullptr);

    virtual ~SocketConfigWidget();

private slots:
    void hostValueChanged(const QString& value);
    void portValueChanged(int value);
    void autoConnectChanged(int value);
    void idChanged(const QString& value);
    void cleanSessionChanged(int value);
    void keepAliveChanged(int value);
    void subTopicsChanged(const QString& value);
    void subQosChanged(int value);
    void pubTopicChanged(const QString& value);
    void pubQosChanged(int value);

private:
    Socket& m_socket;
    Ui::MosquittoSocketConfigWidget m_ui;
};

}  // namespace mosquitto_socket

}  // namespace cc_plugin

}  // namespace mqtt


