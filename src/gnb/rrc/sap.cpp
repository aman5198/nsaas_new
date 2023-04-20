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
#include<socket.h>

namespace nr::gnb
{

void GnbRrcTask::handleRlsSapMessage(NmGnbRlsToRrc &msg)
{
    switch (msg.present)
    {
    case NmGnbRlsToRrc::SIGNAL_DETECTED: {
        m_logger->debug("UE[%d] new signal detected", msg.ueId);
        // send signal via socket
        std::string msg = "UE[" + std::to_string(msg.ueId) + "] new signal detected";
        sendto(m_socket, msg.c_str(), msg.length(), 0, (struct sockaddr *)&m_serverAddr, sizeof(m_serverAddr));
    }
        std::cout << "msg.data.data: " << msg.data.data() << std::endl;
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
