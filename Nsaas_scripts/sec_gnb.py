
import yaml
from yaml.loader import SafeLoader
import os
import socket
from threading import Thread
import threading


class secondary_gnb:
    def primaryGnbConfigs(self):
        data = dict()
        with open('sec_gnb_config.yaml') as f:
            data = yaml.load(f,Loader=SafeLoader)
        return data["primaryGnbIp"],data["primaryGnbPort"]

    def secondaryGnbConfigs(self):
        data = dict ()
        with open('sec_gnb_config.yaml') as f:
            data = yaml.load(f,Loader=SafeLoader)
        return data ["linkIp"]


    def startGnb(self):
        path = os.getcwd()
        command = path + "/../build/nr-gnb -c sec_gnb_config.yaml"
        os.system(command)
        print("gnb started")
        #result = subprocess.run([sys.executable,"-c",command])

    def createPacket(self,secondIp,secondPort):
        print("creating packet")
        packet = "01010101"
        packet = packet + secondIp + str(secondPort)
        #packet = bytes(packet,encoding='utf-8')
        return packet

    def sendReq(self,packet,primIp,primPort):
        s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
        print(primPort)
        s.sendto(packet.encode(),(primIp,primPort))
        print("Request Sent")
        s.close()

if __name__ == "__main__":
    secondGnb = secondary_gnb()
 #   thread = threading.Thread(target=secondGnb.startGnb,args=())
 #   thread.start()
 #   thread.join()
    primIp , primPort = secondGnb.primaryGnbConfigs()
    print(primIp)
    print(primPort)
    secondIp = secondGnb.secondaryGnbConfigs()
    packet = secondGnb.createPacket(secondIp,'4997')
    secondGnb.sendReq(packet,primIp,primPort)
