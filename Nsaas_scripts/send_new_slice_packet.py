import yaml
from yaml.loader import SafeLoader
import os
import socket
from threading import Thread
import threading

class add_new_slice:
    def primaryGnbConfigs(self):
        data = dict()
        with open('sec_gnb_config.yaml') as f:
            data = yaml.load(f,Loader=SafeLoader)
        return data["primaryGnbIp"],data["primaryGnbPort"]
    
    
    def createPacket(self,secondIp,secondPort,sliceSst,sliceSd):
        print("creating packet")
        packet = "01010111"
        packet = packet + secondIp + str(secondPort) + str(sliceSst) + str(sliceSd)
        #packet = bytes(packet,encoding='utf-8')
        return packet

    def sendReq(self,packet,primIp,primPort):
        s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM,0)
        print(primPort)
        s.sendto(packet.encode(),(primIp,primPort))
        print("Request Sent")
        s.close()
        print("socket is closed")

if __name__ == "__main__":
    add_new_slice = add_new_slice()
    primIp , primPort = add_new_slice.primaryGnbConfigs()
    print(primIp)
    print(primPort)
    packet = add_new_slice.createPacket('127.0.0.25','4997','1','000090')
    add_new_slice.sendReq(packet,primIp,primPort)
