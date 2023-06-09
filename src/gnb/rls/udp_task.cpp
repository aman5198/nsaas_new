//
// This file is a part of UERANSIM open source project.
// Copyright (c) 2021 ALİ GÜNGÖR.
//
// The software and all associated files are licensed under GPL-3.0
// and subject to the terms and conditions defined in LICENSE file.
//

#include "udp_task.hpp"
#include <iostream>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <string>
#include <set>
#include "../../utils/sec_gnb.cpp"

#include<gnb/ngap/task.hpp>
#include <sstream>
#include<fstream>
#include <gnb/app/task.hpp>
#include <gnb/sctp/task.hpp>
#include<iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include<string>

#include<gnb/ngap/nas.cpp>
#include <gnb/nts.hpp>
#include <utils/common.hpp>
#include <utils/constants.hpp>
#include <utils/libc_error.hpp>

#include <gnb/gtp/task.hpp>
#include <gnb/gtp/proto.hpp>
#include <gnb/rls/task.hpp>
#include <utils/constants.hpp>
#include <utils/libc_error.hpp>
#include <asn/ngap/ASN_NGAP_QosFlowSetupRequestItem.h>

static constexpr const int BUFFER_SIZE = 16384;

static constexpr const int LOOP_PERIOD = 1000;
static constexpr const int RECEIVE_TIMEOUT = 200;
static constexpr const int HEARTBEAT_THRESHOLD = 2000; // (LOOP_PERIOD + RECEIVE_TIMEOUT)'dan büyük olmalı

static constexpr const int MIN_ALLOWED_DBM = -120;

static int EstimateSimulatedDbm(const Vector3 &myPos, const Vector3 &uePos)
{
    int deltaX = myPos.x - uePos.x;
    int deltaY = myPos.y - uePos.y;
    int deltaZ = myPos.z - uePos.z;

    int distance = static_cast<int>(std::sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ));
    if (distance == 0)
        return -1; // 0 may be confusing for people
    return -distance;
}

namespace nr::gnb
{

RlsUdpTask::RlsUdpTask(TaskBase *base, uint64_t sti, Vector3 phyLocation)
    : m_server{}, m_ctlTask{}, m_sti{sti}, m_phyLocation{phyLocation}, m_lastLoop{}, m_stiToUe{}, m_ueMap{}, m_newIdCounter{}
{
    m_logger = base->logBase->makeUniqueLogger("rls-udp");

    try
    {
        m_server = new udp::UdpServer(base->config->linkIp, cons::RadioLinkPort);
    }
    catch (const LibError &e)
    {
        m_logger->err("RLS failure [%s]", e.what());
        quit();
        return;
    }
}

void RlsUdpTask::onStart()
{
}

void RlsUdpTask::onLoop()
{
    auto current = utils::CurrentTimeMillis();
    if (current - m_lastLoop > LOOP_PERIOD)
    {
        m_lastLoop = current;
        heartbeatCycle(current);
    }

    int ueID;
    int64_t rrcEstablishmentCause;

    std::fstream newfile;
    newfile.open("inlogs.txt",std::ios::in); 
    if (newfile.is_open()){ 

        // assign first value as integer to ueID
        newfile >> ueID;
        // assign third value as integer to w.rrcEstablishmentCause
        newfile >> rrcEstablishmentCause;
        // close the file
        newfile.close();

    }
    //std::cout<<"ueID is "<<ueID<<std::endl;
    //std::cout<<"w.rrcEstablishmentCause is "<<rrcEstablishmentCause<<std::endl;


 //   FILE* infileOctet;
    
    //     // Open person.dat for reading
    // infileOctet = fopen("logsOctet.bin", "wb+");
    // if (infileOctet == NULL) {
    //     fprintf(stderr, "\nError opening file\n");
    //     exit(1);
    // }

    //     const struct  OctetString pdu;
    
    //     // setting pointer to start of the file
    //     rewind(infileOctet);
    
    //     // reading to read_struct
    //     fread(&pdu, sizeof(pdu), 1, infileOctet);

    //     // close file
    //     fclose(infileOctet);
        // printf("%d\n", temp.value);


    FILE* infile;
    
        // Open person.dat for reading
    infile = fopen("logsTmsi.bin", "wb+");
    if (infile == NULL) {
        fprintf(stderr, "\nError opening file\n");
        exit(1);
    }

        struct std::optional<GutiMobileIdentity> temp;
    
        // setting pointer to start of the file
        rewind(infile);
    
        // reading to read_struct
        fread(&temp, sizeof(temp), 1, infile);

        // close file
        fclose(infile);
        // printf("%d\n", temp.value);


    uint8_t buffer[BUFFER_SIZE];
    InetAddress peerAddress;

    int size = m_server->Receive(buffer, BUFFER_SIZE, RECEIVE_TIMEOUT, peerAddress);
    OctetString pdu;
    //struct TaskBase *t;
    //NgapTask temp1(t);
    //temp1.handleInitialNasTransport(ueID, pdu, rrcEstablishmentCause, temp);
    for(int i =0;i<BUFFER_SIZE;i++){
       // std::cout<<buffer[i];
    }
    // Condition to check whether the header in the packet is for secondary_gnb creation
    std::string header = "01010101";
    bool header_match = true;
    for(int i = 0;i<8;i++){
        if(buffer[i]!=header[i]){
            header_match = false;
            break;
        }
    }
    // 01010101127.0.0.324997
    // 127.0.0.32 
    // 4997
    if(header_match){ // parse the packet to add secondary gnb
        std::string ip = "";
        for(int i = 8;i<18;i++){
            ip = ip + (char)buffer[i];
        }
        std::cout<<"ip address of sec gnb is "<<ip<<std::endl;


        std::string port = "";
        for(int i = 18;i<22;i++){
            port = port + (char)buffer[i];
        }
        std::cout<<"port of sec gnb is "<<port<<std::endl;
        addSecGnb(1,1,ip,port);
    }
        
    std::string header_slice = "01010111";
    bool header_slice_match = true;
    for(int i = 0;i<8;i++){
        if(buffer[i]!=header_slice[i]){
            header_slice_match = false;
            break;
        }
    }
    if(header_slice_match){ // parse the packet to add secondary gnb
        std::string ip = "";
        for(int i = 8;i<18;i++){
            ip = ip + (char)buffer[i];
        }
        std::cout<<"ip address of new slice is "<<ip<<std::endl;


        std::string port = "";
        for(int i = 18;i<22;i++){
            port = port + (char)buffer[i];
        }
        std::cout<<"port of new slice is "<<port<<std::endl;

        std::string slice_sst = "";
        for(int i = 22;i<23;i++){
            slice_sst = slice_sst + (char)buffer[i];
        }
        std::cout << "slice sst is " << slice_sst << std::endl;


        std::string slice_sd = "";
        for(int i = 23;i<29;i++){
            slice_sd = slice_sd + (char)buffer[i];
        }
        std::cout << "slice sd is " << slice_sd << std::endl;
        std::string command  = "python3 Nsaas_scripts/add_slice.py --sst " + slice_sst + " --sd " +  slice_sd + " --addr " + ip + " --port " + port;
        std::string terminal_cmd = "gnome-terminal -- bash -c \"" + command + "; exec bash\"";
        system(terminal_cmd.c_str());
        // addSlice(ip,port,slice_sst,slice_sd);
        return;
    }
    // auto &w = dynamic_cast<NmGnbRrcToNgap &>(*msg);
    // w.ueId = 1;
    // handleInitialNasTransport(w.ueId, w.pdu, w.rrcEstablishmentCause, w.sTmsi);
   // std::cout<<std::endl;
    if (size > 0)
    {
        auto rlsMsg = rls::DecodeRlsMessage(OctetView{buffer, static_cast<size_t>(size)});
        if (rlsMsg == nullptr)
            m_logger->debug("Forwarding to 5G Core.");
        else
            receiveRlsPdu(peerAddress, std::move(rlsMsg));
    }
}

void RlsUdpTask::onQuit()
{
    delete m_server;
}

void RlsUdpTask::receiveRlsPdu(const InetAddress &addr, std::unique_ptr<rls::RlsMessage> &&msg)
{
    if (msg->msgType == rls::EMessageType::HEARTBEAT)
    {
        int dbm = EstimateSimulatedDbm(m_phyLocation, ((const rls::RlsHeartBeat &)*msg).simPos);
        if (dbm < MIN_ALLOWED_DBM)
        {
            // if the simulated signal strength is such low, then ignore this message
            return;
        }

        if (m_stiToUe.count(msg->sti))
        {
            int ueId = m_stiToUe[msg->sti];
            m_ueMap[ueId].address = addr;
            m_ueMap[ueId].lastSeen = utils::CurrentTimeMillis();
        }
        else
        {
            int ueId = ++m_newIdCounter;

            m_stiToUe[msg->sti] = ueId;
            m_ueMap[ueId].address = addr;
            m_ueMap[ueId].lastSeen = utils::CurrentTimeMillis();

            auto w = std::make_unique<NmGnbRlsToRls>(NmGnbRlsToRls::SIGNAL_DETECTED);
            w->ueId = ueId;
            m_ctlTask->push(std::move(w));
        }

        rls::RlsHeartBeatAck ack{m_sti};
        ack.dbm = dbm;

        sendRlsPdu(addr, ack);
        return;
    }

    if (!m_stiToUe.count(msg->sti))
    {
        // if no HB received yet, and the message is not HB, then ignore the message
        return;
    }

    auto w = std::make_unique<NmGnbRlsToRls>(NmGnbRlsToRls::RECEIVE_RLS_MESSAGE);
    w->ueId = m_stiToUe[msg->sti];
    w->msg = std::move(msg);
    m_ctlTask->push(std::move(w));
}

void RlsUdpTask::sendRlsPdu(const InetAddress &addr, const rls::RlsMessage &msg)
{
    OctetString stream;
    rls::EncodeRlsMessage(msg, stream);

    m_server->Send(addr, stream.data(), static_cast<size_t>(stream.length()));
}

void RlsUdpTask::heartbeatCycle(int64_t time)
{
    std::set<int> lostUeId{};
    std::set<uint64_t> lostSti{};

    for (auto &item : m_ueMap)
    {
        if (time - item.second.lastSeen > HEARTBEAT_THRESHOLD)
        {
            lostUeId.insert(item.first);
            lostSti.insert(item.second.sti);
        }
    }

    for (uint64_t sti : lostSti)
        m_stiToUe.erase(sti);

    for (int ueId : lostUeId)
        m_ueMap.erase(ueId);

    for (int ueId : lostUeId)
    {
        auto w = std::make_unique<NmGnbRlsToRls>(NmGnbRlsToRls::SIGNAL_LOST);
        w->ueId = ueId;
        m_ctlTask->push(std::move(w));
    }
}

void RlsUdpTask::initialize(NtsTask *ctlTask)
{
    m_ctlTask = ctlTask;
}

void RlsUdpTask::send(int ueId, const rls::RlsMessage &msg)
{
    if (ueId == 0)
    {
        for (auto &ue : m_ueMap)
            send(ue.first, msg);
        return;
    }

    if (!m_ueMap.count(ueId))
    {
        // ignore the message
        return;
    }

    sendRlsPdu(m_ueMap[ueId].address, msg);
}

} // namespace nr::gnb
