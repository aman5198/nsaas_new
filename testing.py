import subprocess
import os
import time
from datetime import datetime


def benignUE():
   # os.system("./build/nr-ue -c config/open5gs-ue.yaml  > log &")
    proc1 = subprocess.run(["./build/nr-ue -c config/open5gs-ue.yaml  > log &"],shell=True)
    #proc1.wait(timeout=5)
   # print(proc1.pid)
    #print(proc.stdout)
    time.sleep(5)
    proc2 = subprocess.Popen("ps -ef | grep './build/nr-ue -c config/open5gs-ue.yaml' | grep -v 'grep' | awk '{ printf $2 }'",shell=True,stdout=subprocess.PIPE)
    str1=str(proc2.stdout)
    pids=""
    for c in iter(lambda:proc2.stdout.read(1),b""):
        pids+=c.decode('utf-8')
    
    print(pids)
    #print("this is the output -------- " + str1)
    subprocess.run(["kill","-9",pids])

    #proc1.kill()

def getTime(logLine):
    logLine=logLine.split()
    print(logLine[1][:-1])
    timeString = logLine[1][:-1]
    format = "%H:%M:%S.%f"

    date_object = datetime.strptime(timeString, format)
    print(date_object)

    return date_object


def timeDiff():
    lookup = "Connection setup for PDU"
    filename = "log"
    f=open(filename)
    lines=f.readlines()
    firstLine =lines[1]
    f.close()

    with open(filename) as myFile:
        for num, line in enumerate(myFile, 1):
            if lookup in line:
                lastLine = line
                #print ("found at line:", num)
    startTime = getTime(firstLine)
    endTime = getTime(lastLine)
    delta = endTime - startTime
    deltams = delta.total_seconds()*1000
    print(deltams)



benignUE()
timeDiff()
