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

#include "SocketRemoteController.h"
#include <errno.h>
#include "main.h"

SocketRemoteController::SocketRemoteController() {
    data[0] = '\0';
    m_port = 58000;
    
    memset((char *)&m_address, 0, sizeof(m_address));
    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = htonl(INADDR_ANY);
    m_address.sin_port = htons((unsigned short) m_port);
}

SocketRemoteController::~SocketRemoteController() {
    close();
}

void SocketRemoteController::set_port(int port) {
    //set the port to desired value
    m_port = port;
}

void SocketRemoteController::open() {

    //Opening socket
    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_socket <= 0) {
        printf("Failed to create socket\n");
        exit(EXIT_FAILURE);
    }

    //int optval = 1;
    //setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

    //Binding to desired port number
    if (bind(m_socket, (struct sockaddr*) &m_address, sizeof (m_address)) < 0) {
        printf("Failed to bind socket\n");
        exit(EXIT_FAILURE);
    }

    //setting Socket to non blocking mode
    int nonBlocking = 1;
    if (fcntl(m_socket, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
        printf("Failed to set non-blocking socket\n");
        exit(EXIT_FAILURE);
    }

    printf("Succeed to create socket\nWaiting for Instructions...\n");
}

void SocketRemoteController::close() {

    if (m_socket != 0) {
        shutdown(m_socket, SHUT_RDWR);
        m_socket = 0;
    }
}

std::string SocketRemoteController::readCommand(sockaddr_in &remoteAddr) {

    if (m_socket == 0)
        printf("Socket is closed...");

    int received_bytes = -1;
    //sockaddr_in from;
    socklen_t fromLength = sizeof (remoteAddr);

    received_bytes = recvfrom(m_socket, data, sizeof (data), 0, (sockaddr*) & remoteAddr, &fromLength);
    if (received_bytes == -1) {
        //printf("received bytes = -1 \n");
        return "";
    }
    data[received_bytes] = 0;
    std::string packet(reinterpret_cast<char const*> (data));

    return packet;
}

void SocketRemoteController::replyCommand(std::string msg, sockaddr_in remoteAddr) {

    if (m_socket == 0)
        printf("Socket is closed...");

    /*
    int ip1 = remoteAddr.sin_addr.s_addr & 0xFF;
    int ip2 = (remoteAddr.sin_addr.s_addr & 0xFF00) >> 8;
    int ip3 = (remoteAddr.sin_addr.s_addr & 0xFF0000) >> 16;
    int ip4 = (remoteAddr.sin_addr.s_addr & 0xFF000000) >> 24;
    std::cout << ip1 << "." << ip2 << "." << ip3 << "." << ip4 << ":" << ntohs(remoteAddr.sin_port) << std::endl;
    */
    //std::cout << "sending " << msg << "  " << msg.length() << std::endl;
    if (sendto(m_socket, msg.c_str(), msg.length(), 0, (sockaddr*) & remoteAddr, (socklen_t) sizeof (remoteAddr)) == -1) {
        std::cerr << "error sendto " << strerror(errno) << std::endl;
    }
}

void SocketRemoteController::start() {
    sockaddr_in remoteAddr;
    while (true) {
        std::string line = readCommand(remoteAddr);
        if (!line.empty()) {
            
            std::cout << line << std::endl;
            
            std::stringstream stream(line);
            int i = 0;
            while (std::getline(stream, stringbuf_[i], ' ')) {
                ++i;
            }

            std::string resp = parser.parse(stringbuf_);
            if (!resp.empty()) {
                replyCommand(resp, remoteAddr);
            }
        } else usleep(10);

    }
    return;
}




