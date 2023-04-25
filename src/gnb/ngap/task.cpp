//
// This file is a part of UERANSIM open source project.
// Copyright (c) 2021 ALİ GÜNGÖR.
//
// The software and all associated files are licensed under GPL-3.0
// and subject to the terms and conditions defined in LICENSE file.
//

#include "task.hpp"

#include <sstream>

#include <gnb/app/task.hpp>
#include <gnb/sctp/task.hpp>
#include<iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<string>


namespace nr::gnb
{

NgapTask::NgapTask(TaskBase *base) : m_base{base}, m_ueNgapIdCounter{}, m_downlinkTeidCounter{}, m_isInitialized{}
{
    m_logger = base->logBase->makeUniqueLogger("ngap");
}

void NgapTask::onStart()
{
    for (auto &amfConfig : m_base->config->amfConfigs)
        createAmfContext(amfConfig);
    if (m_amfCtx.empty())
        m_logger->warn("No AMF configuration is provided");

    for (auto &amfCtx : m_amfCtx)
    {
        auto msg = std::make_unique<NmGnbSctp>(NmGnbSctp::CONNECTION_REQUEST);
        msg->clientId = amfCtx.second->ctxId;
        msg->localAddress = m_base->config->ngapIp;
        msg->localPort = 0;
        std::cout << amfCtx.second->address <<std::endl;
        std::cout << amfCtx.second->port <<std::endl;
        msg->remoteAddress = amfCtx.second->address;
        msg->remotePort = amfCtx.second->port;
        msg->ppid = sctp::PayloadProtocolId::NGAP;
        msg->associatedTask = this;
        m_base->sctpTask->push(std::move(msg));
    }
}

void NgapTask::onLoop()
{
    auto msg = take();
    
    if (!msg)
        return;
    switch (msg->msgType)
    {
    case NtsMessageType::GNB_RRC_TO_NGAP: {
        auto &w = dynamic_cast<NmGnbRrcToNgap &>(*msg);
        switch (w.present)
        {
        case NmGnbRrcToNgap::INITIAL_NAS_DELIVERY: {
 //           std::cout << typeid(w.ueId).name() << std::endl;
 //           std::cout << typeid(w.pdu).name() << std::endl;
  //          std::cout << typeid(w.rrcEstablishmentCause).name() << std::endl;
  //          std::cout << typeid(w.sTmsi).name() << std::endl;
            
            // int m_socket = socket(AF_INET, SOCK_DGRAM, 0);
            // struct sockaddr_in m_serverAddr;
            // m_serverAddr.sin_family = AF_INET;
            // m_serverAddr.sin_port = htons(4997);
            // m_serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
            // socklen_t s_len = sizeof(m_serverAddr);
            // sendto(m_socket, w.sTmsi, 200, 0, (struct sockaddr *)&m_serverAddr, s_len);
            //open file for writing

            FILE* outfile;
  
            // open file for writing
            outfile = fopen("logs.bin", "wb");
            if (outfile == NULL) {
                fprintf(stderr, "\nError opened file\n");
                exit(1);
            }
        
            struct GutiMobileIdentity input1 = w.sTmsi;
        
            // write struct to file
            int flag = 0;
            flag = fwrite(&input1, sizeof(struct GutiMobileIdentity), 1,
                        outfile);
            if (flag) {
                printf("Contents of the structure written "
                    "successfully");
            }
            else
                printf("Error Writing to File!");
        
            // close file
            fclose(outfile);
            handleInitialNasTransport(w.ueId, w.pdu, w.rrcEstablishmentCause, w.sTmsi);
            break;
        }
        case NmGnbRrcToNgap::UPLINK_NAS_DELIVERY: {
            handleUplinkNasTransport(w.ueId, w.pdu);
            break;
        }
        case NmGnbRrcToNgap::RADIO_LINK_FAILURE: {
            handleRadioLinkFailure(w.ueId);
            break;
        }
        }
        break;
    }
    case NtsMessageType::GNB_SCTP: {
        auto &w = dynamic_cast<NmGnbSctp &>(*msg);
        switch (w.present)
        {
        case NmGnbSctp::ASSOCIATION_SETUP:
            handleAssociationSetup(w.clientId, w.associationId, w.inStreams, w.outStreams);
            break;
        case NmGnbSctp::RECEIVE_MESSAGE:
            handleSctpMessage(w.clientId, w.stream, w.buffer);
            break;
        case NmGnbSctp::ASSOCIATION_SHUTDOWN:
            handleAssociationShutdown(w.clientId);
            break;
        default:
            m_logger->unhandledNts(*msg);
            break;
        }
        break;
    }
    default: {
        m_logger->unhandledNts(*msg);
        break;
    }
    }
}

void NgapTask::onQuit()
{
    for (auto &i : m_ueCtx)
        delete i.second;
    for (auto &i : m_amfCtx)
        delete i.second;
    m_ueCtx.clear();
    m_amfCtx.clear();
}

} // namespace nr::gnb
