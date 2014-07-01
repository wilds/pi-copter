/*
 * Copyright (C) 2014 Danilo Selvaggi & Ivano Selvaggi
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SOCKET_REMOTE_CONTROLLER_H
#define SOCKET_REMOTE_CONTROLLER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

#include "CommandParser.h"

class SocketRemoteController {
private:
    sockaddr_in m_address;
    unsigned short m_port;
    int m_socket;
    unsigned char m_lastdata[256];
    std::string readCommand(sockaddr_in &from);
    void replyCommand(std::string data, sockaddr_in to);

    std::string stringbuf_[6];
public:
    SocketRemoteController();
    ~SocketRemoteController();

    void set_port(int port);
    void open();
    void close();
    void start();
    /*unsigned*/ char data[256];
};

#endif
