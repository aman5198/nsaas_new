//
// This file is a part of UERANSIM open source project.
// Copyright (c) 2021 ALİ GÜNGÖR.
//
// The software and all associated files are licensed under GPL-3.0
// and subject to the terms and conditions defined in LICENSE file.
//

#include "task.hpp"

#include <gnb/ngap/task.hpp>
#include <lib/rrc/encode.hpp>
#include<iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace nr::gnb
{

void GnbRrcTask::handleRlsSapMessage(NmGnbRlsToRrc &msg)
{
    switch (msg.present)
    {
    case NmGnbRlsToRrc::SIGNAL_DETECTED: {
        m_logger->debug("UE[%d] new signal detected", msg.ueId);
        // send signal via socket
        std::string msg = "UE: new signal detected";
        int m_socket = socket(AF_INET, SOCK_DGRAM, 0);
        struct sockaddr_in m_serverAddr;
        m_serverAddr.sin_family = AF_INET;
        m_serverAddr.sin_port = htons(4997);
        m_serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
        socklen_t s_len = sizeof(m_serverAddr);
        sendto(m_socket, msg.c_str(), 200, 0, (struct sockaddr *)&m_serverAddr, sizeof(m_serverAddr));
        m_logger->debug("Custom thread Message was sent");
       //std::cout <<"Custom thread Message was sent";
        // std::cout << "msg.data.data: " << msg.data.data() << std::endl;

       // g_cliServer->sendMessage(app::CliMessage::Result(msg.clientAddr, ""));
        triggerSysInfoBroadcast();
        break;
    }
        
    case NmGnbRlsToRrc::UPLINK_RRC: {
        handleUplinkRrc(msg.ueId, msg.rrcChannel, msg.data);
        break;
    }
    }
}

} // namespace nr::gnb
